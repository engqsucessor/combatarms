import { RigidBody } from '@react-three/rapier'

function Map() {
  return (
    <>
      {/* Ground */}
      <RigidBody type="fixed">
        <mesh receiveShadow position={[0, 0, 0]}>
          <boxGeometry args={[50, 1, 50]} />
          <meshStandardMaterial color="#555" />
        </mesh>
      </RigidBody>

      {/* Walls - Cabin structure */}
      {/* Front wall with door */}
      <RigidBody type="fixed">
        <mesh position={[0, 3, -10]} castShadow receiveShadow>
          <boxGeometry args={[6, 6, 0.5]} />
          <meshStandardMaterial color="#8B4513" />
        </mesh>
      </RigidBody>

      <RigidBody type="fixed">
        <mesh position={[8, 3, -10]} castShadow receiveShadow>
          <boxGeometry args={[6, 6, 0.5]} />
          <meshStandardMaterial color="#8B4513" />
        </mesh>
      </RigidBody>

      {/* Back wall */}
      <RigidBody type="fixed">
        <mesh position={[0, 3, 10]} castShadow receiveShadow>
          <boxGeometry args={[20, 6, 0.5]} />
          <meshStandardMaterial color="#8B4513" />
        </mesh>
      </RigidBody>

      {/* Left wall */}
      <RigidBody type="fixed">
        <mesh position={[-10, 3, 0]} castShadow receiveShadow>
          <boxGeometry args={[0.5, 6, 20]} />
          <meshStandardMaterial color="#8B4513" />
        </mesh>
      </RigidBody>

      {/* Right wall */}
      <RigidBody type="fixed">
        <mesh position={[10, 3, 0]} castShadow receiveShadow>
          <boxGeometry args={[0.5, 6, 20]} />
          <meshStandardMaterial color="#8B4513" />
        </mesh>
      </RigidBody>

      {/* Crates for cover */}
      <RigidBody type="fixed">
        <mesh position={[-5, 1, -5]} castShadow receiveShadow>
          <boxGeometry args={[2, 2, 2]} />
          <meshStandardMaterial color="#654321" />
        </mesh>
      </RigidBody>

      <RigidBody type="fixed">
        <mesh position={[5, 1, 5]} castShadow receiveShadow>
          <boxGeometry args={[2, 2, 2]} />
          <meshStandardMaterial color="#654321" />
        </mesh>
      </RigidBody>
    </>
  )
}

export default Map
