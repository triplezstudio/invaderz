
#include "RegularGameStage.hh"
#include "Constants.hh"
#include "CoordinateConverter.hh"
#include "Effects.hh"
#include <format>

namespace invaderz {

RegularGameStage::RegularGameStage(GameState *state, Eigen::Vector3f screenDims)
  : IStage(state)
  , runtime::CoreObject("game")
  , m_screenDims(screenDims)
{
  initialize(std::move(screenDims));
}

void RegularGameStage::loadSounds(IAudioRegistry &registry)
{
  auto fireFilePath = std::format("{}/fire_sound.ogg", std::getenv("ASSET_FOLDER"));
  m_fireSound       = registry.registerSound(fireFilePath);

  auto explosionFilePath = std::format("{}/explosion_sound.ogg", std::getenv("ASSET_FOLDER"));
  m_explosionSound       = registry.registerSound(explosionFilePath);
}

void RegularGameStage::loadTextures(ITextureRegistry &registry)
{
  auto backgroundFilePath = std::format("{}/star_background.png", std::getenv("ASSET_FOLDER"));
  m_background            = registry.registerTexture(backgroundFilePath);

  auto spaceshipFilePath = std::format("{}/player_ship.png", std::getenv("ASSET_FOLDER"));
  m_spaceShip            = registry.registerTexture(spaceshipFilePath);

  auto enemyShipFilePath = std::format("{}/enemy_ship.png", std::getenv("ASSET_FOLDER"));
  m_enemyShip            = registry.registerTexture(enemyShipFilePath);

  auto bulletFilePath = std::format("{}/bullet.png", std::getenv("ASSET_FOLDER"));
  m_bullet            = registry.registerTexture(bulletFilePath);

  auto enemyBulletFilePath = std::format("{}/enemy_bullet.png", std::getenv("ASSET_FOLDER"));
  m_enemyBullet            = registry.registerTexture(enemyBulletFilePath);

  auto explosionFilePath = std::format("{}/explosion.png", std::getenv("ASSET_FOLDER"));
  m_explosion            = registry.registerTexture(explosionFilePath);
}

void RegularGameStage::loadFonts(IFontRegistry &registry)
{
  auto filePath = std::format("{}/ArcadeClassic.ttf", std::getenv("ASSET_FOLDER"));
  m_font        = registry.registerFont(filePath, 16);

  filePath    = std::format("{}/ArcadeClassic.ttf", std::getenv("ASSET_FOLDER"));
  m_debugFont = registry.registerFont(filePath, 10);

  // Theoretically it would be better to check that sounds, textures and fonts have
  // been loaded but we use this as a proxy because it's the last function called by
  // the StageFactory.
  m_subscreen = Subscreen::RUN;
}

namespace {
const Eigen::Vector3f SCORE_MENU_SIZE(0.0f, 64.0f, 0.0);
}

auto RegularGameStage::update(const FrameData &data) -> StageResult
{
  if (m_subscreen == Subscreen::RUN)
  {
    Effects effects{};
    m_playerUpdater->update(data, effects);

    m_level->update(*m_state, data.elapsed, effects);

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
      CoordinateConverter converter{m_level->dims(), screenOffset, displayDims};

      m_explosions.emplace_back(converter.toScreenPos(explosion, explosionDimensions()));
      ++m_explosionsToStart;
    }

    // Handle end game
    if (m_state->lives <= 0 || m_level->remainingWaves() <= 0)
    {
      if (m_state->lives > 0)
      {
        ++m_state->level;
      }

      if (m_state->level > m_state->maxLevel || m_state->lives <= 0)
      {
        info("Transition from game screen to end game screen");
        return StageResult{.finished = true, .next = Screen::END_GAME};
      }

      if ((m_state->level + 1) % m_state->bossLevelInterval == 0)
      {
        info("Transition from regular level to boss level");
        return StageResult{.finished = true, .next = Screen::BOSS_LEVEL};
      }

      info("Reached next regular level");
      return StageResult{.finished = true, .next = Screen::LEVEL};
    }
  }

  processActionKeys(data);

  return overrideWithEscapeKey(data, StageResult{});
}

void RegularGameStage::processSounds(IAudioEngine &engine)
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

void RegularGameStage::render(IRenderer &renderer, const FrameData &data)
{
  renderer.renderTexture(m_background, Eigen::Vector3f::Zero(), m_screenDims);

  Eigen::Vector3f screenOffset(0.0f, SCORE_MENU_SIZE(1), 0.0f);
  Eigen::Vector3f displayDims = m_screenDims - SCORE_MENU_SIZE;
  CoordinateConverter converter{m_level->dims(), screenOffset, displayDims};

  renderer.renderTexture(m_spaceShip,
                         converter.toScreenPos(m_level->playerPosition(), playerDimensions()),
                         playerDimensions());

  renderExplosions(renderer);

  for (const auto &enemy : m_level->enemies())
  {
    renderer.renderTexture(m_enemyShip,
                           converter.toScreenPos(enemy, enemyDimensions()),
                           enemyDimensions());
  }

  for (const auto &bullet : m_level->bullets())
  {
    if (bullet.canDamagePlayer)
    {
      renderer.renderTexture(m_enemyBullet,
                             converter.toScreenPos(bullet.pos, bulletDimensions()),
                             bulletDimensions(),
                             90.0f);
    }
    else
    {
      renderer.renderTexture(m_bullet,
                             converter.toScreenPos(bullet.pos, bulletDimensions()),
                             bulletDimensions(),
                             -90.0f);
    }
  }

  renderScoreMenu(renderer, data);

  if (m_subscreen == Subscreen::PAUSE)
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

void RegularGameStage::initialize(Eigen::Vector3f screenDims)
{
  Eigen::Vector3f worldDims = screenDims - SCORE_MENU_SIZE;
  m_level         = std::make_unique<Level>(m_state->level + 1, WAVES_COUNT, std::move(worldDims));
  m_playerUpdater = std::make_unique<PlayerUpdater>(*m_level);
}

void RegularGameStage::processActionKeys(const FrameData &data)
{
  if (data.state.released(keyboard::P))
  {
    switch (m_subscreen)
    {
      case Subscreen::LOAD:
        break;
      case Subscreen::RUN:
        info("Transition from game screen to pause screen");
        m_subscreen = Subscreen::PAUSE;
        break;
      case Subscreen::PAUSE:
        info("Transition from pause screen to game screen");
        m_subscreen = Subscreen::RUN;
        break;
      default:
        break;
    }
  }
}

void RegularGameStage::renderScoreMenu(IRenderer &renderer, const FrameData &data)
{
  Eigen::Vector3f position(10.0f, 10.0f, 0.0f);
  auto text = std::format("Score {}", m_state->score);
  renderer.renderText(m_font, text, position);

  position = Eigen::Vector3f(m_screenDims(0) / 3.0f, 10.0f, 0.0f);
  text     = std::format("Lives {}", m_state->lives);
  renderer.renderText(m_font, text, position);

  // TODO: Could probably be moved to a member function in the GameState
  if ((m_state->level + 1) % m_state->bossLevelInterval == 0)
  {
    position = Eigen::Vector3f(2.0f * m_screenDims(0) / 3.0f, 10.0f, 0.0f);
    renderer.renderText(m_font, "BOSS !!!", position);
  }

  position = Eigen::Vector3f(2.0f, m_screenDims(1) - 30.0f, 0.0f);
  text     = std::format("frame time: {}", data.frameTimeMicroseconds());
  renderer.renderText(m_debugFont, text, position);
}

void RegularGameStage::renderExplosions(IRenderer &renderer)
{
  for (auto &explosion : m_explosions)
  {
    explosion.render(renderer, m_explosion);
  }
}

} // namespace invaderz
