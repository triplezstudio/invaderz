
#include "GameStage.hh"
#include "Constants.hh"
#include "CoordinateConverter.hh"
#include "Effects.hh"
#include <format>

namespace invaderz {

GameStage::GameStage(GameState *state, Eigen::Vector3f screenDims)
  : IStage(state)
  , runtime::CoreObject("game")
  , m_screenDims(screenDims)
{
  initialize(std::move(screenDims));
}

void GameStage::loadSounds(IAudioRegistry &registry)
{
  auto fireFilePath = std::format("{}/fire_sound.ogg", std::getenv("ASSET_FOLDER"));
  m_fireSound       = registry.registerSound(fireFilePath);

  auto explosionFilePath = std::format("{}/explosion_sound.ogg", std::getenv("ASSET_FOLDER"));
  m_explosionSound       = registry.registerSound(explosionFilePath);
}

void GameStage::loadTextures(ITextureRegistry &registry)
{
  auto backgroundFilePath = std::format("{}/star_background.png", std::getenv("ASSET_FOLDER"));
  m_background            = registry.registerTexture(backgroundFilePath);

  auto spaceshipFilePath = std::format("{}/player_ship.png", std::getenv("ASSET_FOLDER"));
  m_spaceShip            = registry.registerTexture(spaceshipFilePath);

  auto enemyShipFilePath = std::format("{}/enemy_ship.png", std::getenv("ASSET_FOLDER"));
  m_enemyShip            = registry.registerTexture(enemyShipFilePath);

  auto bulletFilePath = std::format("{}/bullet.png", std::getenv("ASSET_FOLDER"));
  m_bullet            = registry.registerTexture(bulletFilePath);

  auto explosionFilePath = std::format("{}/explosion.png", std::getenv("ASSET_FOLDER"));
  m_explosion            = registry.registerTexture(explosionFilePath);
}

void GameStage::loadFonts(IFontRegistry &registry)
{
  auto filePath = std::format("{}/ArcadeClassic.ttf", std::getenv("ASSET_FOLDER"));
  m_font        = registry.registerFont(filePath, 16);
}

namespace {
const Eigen::Vector3f SCORE_MENU_SIZE(0.0f, 64.0f, 0.0);
}

auto GameStage::update(const FrameData &data) -> StageResult
{
  Effects effects{};
  m_playerUpdater->update(data, effects);

  m_world->update(data.elapsed, effects);

  // Process shots
  m_shotsToFire += static_cast<int>(effects.shots.size());

  // Process explosions
  for (auto &explosion : m_explosions)
  {
    explosion.update(data.elapsed);
  }
  std::erase_if(m_explosions, [](const Explosion &e) { return e.finished(); });
  for (const auto &explosion : effects.explosions)
  {
    Eigen::Vector3f screenOffset(0.0f, SCORE_MENU_SIZE(1), 0.0f);
    Eigen::Vector3f displayDims = m_screenDims - SCORE_MENU_SIZE;
    CoordinateConverter converter{m_world->dims(), screenOffset, displayDims};

    m_explosions.emplace_back(converter.toScreenPos(explosion, explosionDimensions()));
    ++m_explosionsToStart;
  }

  // Handle end game
  if (m_state->lives <= 0 || m_world->remainingWaves() <= 0)
  {
    info("Transition from game screen to end game screen");
    return StageResult{.finished = true, .next = Screen::END_GAME};
  }

  processActionKeys(data);

  return overrideWithEscapeKey(data, StageResult{});
}

void GameStage::processSounds(IAudioEngine &engine)
{
  while (m_shotsToFire > 0)
  {
    engine.playOnce(m_fireSound, 0.05f);
    --m_shotsToFire;
  }

  while (m_explosionsToStart > 0)
  {
    engine.playOnce(m_explosionSound, 0.05f);
    --m_explosionsToStart;
  }
}

namespace {
constexpr auto PAUSE_TEXT = "PAUSE";
}

void GameStage::render(IRenderer &renderer)
{
  renderer.renderTexture(m_background, Eigen::Vector3f::Zero(), m_screenDims);

  Eigen::Vector3f screenOffset(0.0f, SCORE_MENU_SIZE(1), 0.0f);
  Eigen::Vector3f displayDims = m_screenDims - SCORE_MENU_SIZE;
  CoordinateConverter converter{m_world->dims(), screenOffset, displayDims};

  renderer.renderTexture(m_spaceShip,
                         converter.toScreenPos(m_world->playerPosition(), playerDimensions()),
                         playerDimensions());

  renderExplosions(renderer);

  for (const auto &enemy : m_world->enemies())
  {
    renderer.renderTexture(m_enemyShip,
                           converter.toScreenPos(enemy, enemyDimensions()),
                           enemyDimensions());
  }

  for (const auto &bullet : m_world->bullets())
  {
    renderer.renderTexture(m_bullet,
                           converter.toScreenPos(bullet, bulletDimensions()),
                           bulletDimensions(),
                           -90.0f);
  }

  renderScoreMenu(renderer);

  if (m_paused)
  {
    const auto &font          = renderer.getFontRegistry().getFont(m_font);
    const auto textDimensions = font.getTextSize(PAUSE_TEXT);
    Eigen::Vector3f position((m_screenDims(0) - textDimensions(0)) / 2.0f, 400.0f, 0.0f);
    renderer.renderText(m_font, PAUSE_TEXT, position);
  }
}

namespace {
constexpr auto WAVES_COUNT = 5;
} // namespace

void GameStage::initialize(Eigen::Vector3f screenDims)
{
  Eigen::Vector3f worldDims = screenDims - SCORE_MENU_SIZE;

  Level level(WAVES_COUNT, std::move(worldDims));
  m_world         = std::make_unique<World>(m_state, std::move(level));
  m_playerUpdater = std::make_unique<PlayerUpdater>(*m_world);
  m_explosions.clear();
  m_state->reset();
}

void GameStage::processActionKeys(const FrameData &data)
{
  if (data.state.released(keyboard::P))
  {
    if (m_paused)
    {
      info("Transition from pause screen to game screen");
    }
    else if (!m_paused)
    {
      info("Transition from game screen to pause screen");
    }
    m_paused = !m_paused;
  }
}

void GameStage::renderScoreMenu(IRenderer &renderer)
{
  Eigen::Vector3f position(10.0f, 10.0f, 0.0f);
  auto text = std::format("Score {}", m_state->score);
  renderer.renderText(m_font, text, position);

  position = Eigen::Vector3f(m_screenDims(0) / 2.0f, 10.0f, 0.0f);
  text     = std::format("Lives {}", m_state->lives);
  renderer.renderText(m_font, text, position);
}

void GameStage::renderExplosions(IRenderer &renderer)
{
  for (auto &explosion : m_explosions)
  {
    explosion.render(renderer, m_explosion);
  }
}

} // namespace invaderz
