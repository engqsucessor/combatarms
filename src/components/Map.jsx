import { RigidBody } from '@react-three/rapier'

function Map() {
  return (
    <>
      {/* Ground - Large arena */}
      <RigidBody type="fixed">
        <mesh receiveShadow position={[0, -0.5, 0]}>
          <boxGeometry args={[100, 1, 100]} />
          <meshStandardMaterial color="#3a3a3a" />
        </mesh>
      </RigidBody>

      {/* Central Cabin Structure */}
      {/* Floor */}
      <RigidBody type="fixed">
        <mesh receiveShadow position={[0, 0.1, 0]}>
          <boxGeometry args={[16, 0.2, 16]} />
          <meshStandardMaterial color="#6b4423" />
        </mesh>
      </RigidBody>

      {/* Front wall segments (with doors) */}
      <RigidBody type="fixed">
        <mesh position={[-5, 3, -8]} castShadow receiveShadow>
          <boxGeometry args={[6, 6, 0.5]} />
          <meshStandardMaterial color="#8B4513" roughness={0.9} />
        </mesh>
      </RigidBody>

      <RigidBody type="fixed">
        <mesh position={[5, 3, -8]} castShadow receiveShadow>
          <boxGeometry args={[6, 6, 0.5]} />
          <meshStandardMaterial color="#8B4513" roughness={0.9} />
        </mesh>
      </RigidBody>

      {/* Back wall */}
      <RigidBody type="fixed">
        <mesh position={[0, 3, 8]} castShadow receiveShadow>
          <boxGeometry args={[16, 6, 0.5]} />
          <meshStandardMaterial color="#8B4513" roughness={0.9} />
        </mesh>
      </RigidBody>

      {/* Left wall with window */}
      <RigidBody type="fixed">
        <mesh position={[-8, 2, 0]} castShadow receiveShadow>
          <boxGeometry args={[0.5, 4, 8]} />
          <meshStandardMaterial color="#8B4513" roughness={0.9} />
        </mesh>
      </RigidBody>

      <RigidBody type="fixed">
        <mesh position={[-8, 5, 0]} castShadow receiveShadow>
          <boxGeometry args={[0.5, 2, 16]} />
          <meshStandardMaterial color="#8B4513" roughness={0.9} />
        </mesh>
      </RigidBody>

      {/* Right wall with window */}
      <RigidBody type="fixed">
        <mesh position={[8, 2, 0]} castShadow receiveShadow>
          <boxGeometry args={[0.5, 4, 8]} />
          <meshStandardMaterial color="#8B4513" roughness={0.9} />
        </mesh>
      </RigidBody>

      <RigidBody type="fixed">
        <mesh position={[8, 5, 0]} castShadow receiveShadow>
          <boxGeometry args={[0.5, 2, 16]} />
          <meshStandardMaterial color="#8B4513" roughness={0.9} />
        </mesh>
      </RigidBody>

      {/* Roof */}
      <RigidBody type="fixed">
        <mesh position={[0, 6.5, 0]} castShadow receiveShadow>
          <boxGeometry args={[17, 0.5, 17]} />
          <meshStandardMaterial color="#5a3a1a" roughness={0.9} />
        </mesh>
      </RigidBody>

      {/* Cover objects inside cabin */}
      <RigidBody type="fixed">
        <mesh position={[-4, 1, -4]} castShadow receiveShadow>
          <boxGeometry args={[2, 2, 2]} />
          <meshStandardMaterial color="#654321" />
        </mesh>
      </RigidBody>

      <RigidBody type="fixed">
        <mesh position={[4, 1, 4]} castShadow receiveShadow>
          <boxGeometry args={[2, 2, 2]} />
          <meshStandardMaterial color="#654321" />
        </mesh>
      </RigidBody>

      <RigidBody type="fixed">
        <mesh position={[0, 0.75, 0]} castShadow receiveShadow>
          <boxGeometry args={[3, 1.5, 3]} />
          <meshStandardMaterial color="#7a5a3a" />
        </mesh>
      </RigidBody>

      {/* Barrels outside */}
      <RigidBody type="fixed">
        <mesh position={[-12, 1, -12]} castShadow receiveShadow>
          <cylinderGeometry args={[0.5, 0.5, 2]} />
          <meshStandardMaterial color="#444" metalness={0.7} />
        </mesh>
      </RigidBody>

      <RigidBody type="fixed">
        <mesh position={[12, 1, 12]} castShadow receiveShadow>
          <cylinderGeometry args={[0.5, 0.5, 2]} />
          <meshStandardMaterial color="#444" metalness={0.7} />
        </mesh>
      </RigidBody>

      <RigidBody type="fixed">
        <mesh position={[-12, 1, 12]} castShadow receiveShadow>
          <cylinderGeometry args={[0.5, 0.5, 2]} />
          <meshStandardMaterial color="#444" metalness={0.7} />
        </mesh>
      </RigidBody>

      <RigidBody type="fixed">
        <mesh position={[12, 1, -12]} castShadow receiveShadow>
          <cylinderGeometry args={[0.5, 0.5, 2]} />
          <meshStandardMaterial color="#444" metalness={0.7} />
        </mesh>
      </RigidBody>
    </>
  )
}

export default Map
