import { useState } from 'react'
import Game from './Game'
import MainMenu from './components/UI/MainMenu'

function App() {
  const [gameStarted, setGameStarted] = useState(false)

  return (
    <>
      {!gameStarted ? (
        <MainMenu onStart={() => setGameStarted(true)} />
      ) : (
        <Game />
      )}
    </>
  )
}

export default App
