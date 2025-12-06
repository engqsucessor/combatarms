import { useGameStore } from '../../stores/gameStore'
import './GameHUD.css'

function GameHUD() {
  const {
    health,
    armor,
    currentAmmo,
    reserveAmmo,
    wave,
    kills,
    zombiesRemaining,
    currentWeapon,
    isReloading
  } = useGameStore()

  return (
    <div className="hud">
      {/* Crosshair */}
      <div className="crosshair">
        <div className="crosshair-line crosshair-top"></div>
        <div className="crosshair-line crosshair-bottom"></div>
        <div className="crosshair-line crosshair-left"></div>
        <div className="crosshair-line crosshair-right"></div>
      </div>

      {/* Bottom Left - Health/Armor */}
      <div className="bottom-left">
        <div className="stat-bar">
          <div className="stat-label">HP</div>
          <div className="bar-container">
            <div className="bar bar-health" style={{ width: `${health}%` }}></div>
          </div>
          <div className="stat-value">{Math.round(health)}</div>
        </div>

        <div className="stat-bar">
          <div className="stat-label">AP</div>
          <div className="bar-container">
            <div className="bar bar-armor" style={{ width: `${armor}%` }}></div>
          </div>
          <div className="stat-value">{Math.round(armor)}</div>
        </div>
      </div>

      {/* Bottom Right - Ammo */}
      <div className="bottom-right">
        <div className="weapon-info">
          <div className="weapon-name">{currentWeapon}</div>
          {isReloading ? (
            <div className="ammo-display reloading">RELOADING...</div>
          ) : (
            <div className="ammo-display">
              <span className="current-ammo">{currentAmmo}</span>
              <span className="ammo-separator">/</span>
              <span className="reserve-ammo">{reserveAmmo}</span>
            </div>
          )}
        </div>
      </div>

      {/* Top Center - Wave Info */}
      <div className="top-center">
        <div className="wave-info">
          <div className="wave-number">WAVE {wave}</div>
          <div className="zombies-remaining">Zombies: {zombiesRemaining}</div>
        </div>
      </div>

      {/* Top Left - Stats */}
      <div className="top-left">
        <div className="stat-item">Kills: {kills}</div>
      </div>

      {/* Instructions */}
      <div className="instructions">
        <div>WASD - Move | Shift - Sprint | Ctrl - Crouch | Space - Jump</div>
        <div>Mouse - Aim | Left Click - Shoot | R - Reload | ESC - Menu</div>
      </div>
    </div>
  )
}

export default GameHUD
