import { useRef, useState, useEffect } from 'react'
import { useFrame } from '@react-three/fiber'
import { RigidBody } from '@react-three/rapier'
import { useGameStore } from '../stores/gameStore'
import * as THREE from 'three'

function Zombie({ position, id, playerRef }) {
  const zombieRef = useRef()
  const [health, setHealth] = useState(100)
  const [isDead, setIsDead] = useState(false)
  const addKill = useGameStore(state => state.addKill)

  // Chase player
  useFrame((state, delta) => {
    if (!zombieRef.current || isDead || !playerRef?.current) return

    const zombiePos = zombieRef.current.translation()
    const playerPos = playerRef.current.translation()

    // Calculate direction to player
    const direction = new THREE.Vector3(
      playerPos.x - zombiePos.x,
      0,
      playerPos.z - zombiePos.z
    ).normalize()

    const speed = 2.5 // Zombie move speed

    // Move towards player
    zombieRef.current.setLinvel({
      x: direction.x * speed,
      y: zombieRef.current.linvel().y,
      z: direction.z * speed
    }, true)

    // Attack player if close enough
    const distance = Math.sqrt(
      Math.pow(playerPos.x - zombiePos.x, 2) +
      Math.pow(playerPos.z - zombiePos.z, 2)
    )

    if (distance < 2) {
      // Damage player (could add cooldown here)
      useGameStore.getState().takeDamage(0.2) // 0.2 damage per frame when close
    }
  })

  // Take damage
  useEffect(() => {
    const handleDamage = (event) => {
      if (event.detail.zombieId === id && !isDead) {
        const newHealth = health - event.detail.damage
        setHealth(newHealth)

        if (newHealth <= 0) {
          setIsDead(true)
          addKill()

          // Notify death
          const deathEvent = new CustomEvent('zombieDeath', {
            detail: { zombieId: id }
          })
          window.dispatchEvent(deathEvent)

          // Remove zombie after death animation
          setTimeout(() => {
            if (zombieRef.current) {
              zombieRef.current.setEnabled(false)
            }
          }, 1000)
        }
      }
    }

    window.addEventListener('zombieDamage', handleDamage)
    return () => window.removeEventListener('zombieDamage', handleDamage)
  }, [health, isDead, id, addKill])

  if (isDead) {
    return null
  }

  return (
    <RigidBody
      ref={zombieRef}
      position={position}
      type="dynamic"
      enabledRotations={[false, false, false]}
      lockRotations
    >
      {/* Zombie body */}
      <group userData={{ type: 'zombie', id: id }}>
        {/* Head */}
        <mesh position={[0, 1.6, 0]} castShadow receiveShadow>
          <sphereGeometry args={[0.25, 16, 16]} />
          <meshStandardMaterial
            color="#4a6b3a"
            roughness={0.9}
            metalness={0.1}
            emissive="#1a2a0a"
            emissiveIntensity={0.1}
          />
        </mesh>

        {/* Eyes (red glow) */}
        <mesh position={[-0.1, 1.65, 0.2]} castShadow>
          <sphereGeometry args={[0.04, 8, 8]} />
          <meshStandardMaterial
            color="#ff0000"
            emissive="#ff0000"
            emissiveIntensity={2}
          />
        </mesh>
        <mesh position={[0.1, 1.65, 0.2]} castShadow>
          <sphereGeometry args={[0.04, 8, 8]} />
          <meshStandardMaterial
            color="#ff0000"
            emissive="#ff0000"
            emissiveIntensity={2}
          />
        </mesh>

        {/* Torso */}
        <mesh position={[0, 1, 0]} castShadow receiveShadow>
          <boxGeometry args={[0.5, 0.7, 0.25]} />
          <meshStandardMaterial
            color="#3a5a2a"
            roughness={0.8}
            metalness={0.2}
          />
        </mesh>

        {/* Arms */}
        <mesh position={[-0.35, 1, 0]} castShadow receiveShadow rotation={[0, 0, 0.3]}>
          <cylinderGeometry args={[0.08, 0.08, 0.6, 8]} />
          <meshStandardMaterial color="#3a5a2a" roughness={0.9} />
        </mesh>
        <mesh position={[0.35, 1, 0]} castShadow receiveShadow rotation={[0, 0, -0.3]}>
          <cylinderGeometry args={[0.08, 0.08, 0.6, 8]} />
          <meshStandardMaterial color="#3a5a2a" roughness={0.9} />
        </mesh>

        {/* Legs */}
        <mesh position={[-0.15, 0.35, 0]} castShadow receiveShadow>
          <cylinderGeometry args={[0.09, 0.09, 0.7, 8]} />
          <meshStandardMaterial color="#2a4a1a" roughness={0.9} />
        </mesh>
        <mesh position={[0.15, 0.35, 0]} castShadow receiveShadow>
          <cylinderGeometry args={[0.09, 0.09, 0.7, 8]} />
          <meshStandardMaterial color="#2a4a1a" roughness={0.9} />
        </mesh>

        {/* Green glow effect */}
        <pointLight
          position={[0, 1, 0]}
          intensity={0.5}
          distance={3}
          color="#00ff00"
          decay={2}
        />

        {/* Health bar above head */}
        {health < 100 && (
          <group position={[0, 2.2, 0]}>
            <mesh position={[0, 0, 0]}>
              <planeGeometry args={[1, 0.1]} />
              <meshBasicMaterial color="#000" />
            </mesh>
            <mesh position={[-(1 - health/100)/2, 0, 0.01]}>
              <planeGeometry args={[health/100, 0.08]} />
              <meshBasicMaterial color="#00ff00" />
            </mesh>
          </group>
        )}
      </group>
    </RigidBody>
  )
}

export default Zombie
