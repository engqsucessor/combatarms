import './MainMenu.css'

function MainMenu({ onStart }) {
  return (
    <div className="main-menu">
      <div className="menu-container">
        <h1 className="game-title">COMBAT ARMS</h1>
        <h2 className="game-subtitle">Cabin Fever</h2>

        <div className="menu-buttons">
          <button className="menu-button" onClick={onStart}>
            START GAME
          </button>
          <button className="menu-button" disabled>
            LOADOUT (COMING SOON)
          </button>
          <button className="menu-button" disabled>
            OPTIONS (COMING SOON)
          </button>
        </div>

        <div className="menu-info">
          <p>Browser-based FPS zombie survival</p>
          <p>Click START GAME and then click to lock mouse</p>
        </div>
      </div>
    </div>
  )
}

export default MainMenu
