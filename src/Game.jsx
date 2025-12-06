import { useRef } from 'react'
import { Canvas } from '@react-three/fiber'
import { Physics } from '@react-three/rapier'
import { PointerLockControls, Sky, Environment } from '@react-three/drei'
import Player from './components/Player'
import Map from './components/Map'
import WaveSpawner from './components/WaveSpawner'
import GameHUD from './components/UI/GameHUD'
import Effects from './components/Effects'

function Game() {
  const playerRef = useRef()

  return (
    <>
      {/* 3D Canvas */}
      <Canvas
        shadows
        camera={{ fov: 75, near: 0.1, far: 1000 }}
        style={{ height: '100vh', width: '100vw' }}
        gl={{
          antialias: true,
          powerPreference: 'high-performance',
          alpha: false,
          stencil: false
        }}
      >
        {/* Skybox */}
        <Sky
          sunPosition={[100, 20, 100]}
          turbidity={8}
          rayleigh={2}
          mieCoefficient={0.005}
          mieDirectionalG={0.8}
        />

        {/* HDR Environment for realistic lighting */}
        <Environment preset="sunset" />

        {/* Fog for atmosphere */}
        <fog attach="fog" args={['#1a1a1a', 30, 100]} />

        {/* Better lighting setup */}
        <ambientLight intensity={0.3} color="#ffffff" />

        {/* Main directional light (sun) */}
        <directionalLight
          position={[20, 30, 10]}
          intensity={1.5}
          castShadow
          shadow-mapSize-width={4096}
          shadow-mapSize-height={4096}
          shadow-camera-far={100}
          shadow-camera-left={-50}
          shadow-camera-right={50}
          shadow-camera-top={50}
          shadow-camera-bottom={-50}
          shadow-bias={-0.0001}
        />

        {/* Fill lights for softer shadows */}
        <directionalLight
          position={[-10, 10, -10]}
          intensity={0.3}
          color="#6699ff"
        />

        {/* Point lights around the map for ambiance */}
        <pointLight position={[0, 4, 0]} intensity={0.5} color="#ff9955" distance={20} />
        <pointLight position={[-15, 3, -15]} intensity={0.3} color="#ff6633" distance={15} />
        <pointLight position={[15, 3, 15]} intensity={0.3} color="#ff6633" distance={15} />

        <Physics gravity={[0, -30, 0]}>
          <Player ref={playerRef} />
          <Map />
          <WaveSpawner playerRef={playerRef} />
        </Physics>

        <PointerLockControls />

        {/* Post-processing effects */}
        <Effects />
      </Canvas>

      {/* HUD Overlay */}
      <GameHUD />
    </>
  )
}

export default Game
