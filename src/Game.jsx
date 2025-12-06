import { Canvas } from '@react-three/fiber'
import { Physics } from '@react-three/rapier'
import { PointerLockControls, Sky } from '@react-three/drei'
import Player from './components/Player'
import Map from './components/Map'
import GameHUD from './components/UI/GameHUD'
import { useGameStore } from './stores/gameStore'

function Game() {
  return (
    <>
      {/* 3D Canvas */}
      <Canvas
        shadows
        camera={{ fov: 75, near: 0.1, far: 1000 }}
        style={{ height: '100vh', width: '100vw' }}
      >
        <Sky sunPosition={[100, 20, 100]} />
        <ambientLight intensity={0.5} />
        <directionalLight
          position={[10, 20, 10]}
          intensity={1}
          castShadow
          shadow-mapSize-width={2048}
          shadow-mapSize-height={2048}
        />

        <Physics gravity={[0, -30, 0]}>
          <Player />
          <Map />
        </Physics>

        <PointerLockControls />
      </Canvas>

      {/* HUD Overlay */}
      <GameHUD />
    </>
  )
}

export default Game
