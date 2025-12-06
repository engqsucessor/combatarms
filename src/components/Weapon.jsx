import { useRef, useEffect, useState } from 'react'
import { useFrame, useThree } from '@react-three/fiber'
import { useGameStore } from '../stores/gameStore'
import * as THREE from 'three'

function Weapon() {
  const weaponRef = useRef()
  const muzzleFlashRef = useRef()
  const { camera, raycaster, scene } = useThree()
  const { shoot, reload, currentAmmo, isReloading } = useGameStore()
  const [recoil, setRecoil] = useState(0)

  // Position weapon in front of camera
  useFrame(() => {
    if (!weaponRef.current) return

    weaponRef.current.position.copy(camera.position)
    weaponRef.current.position.add(
      new THREE.Vector3(0.3, -0.3, -0.5).applyQuaternion(camera.quaternion)
    )
    weaponRef.current.quaternion.copy(camera.quaternion)

    // Recoil recovery
    if (recoil > 0) {
      setRecoil(r => Math.max(0, r - 0.1))
      camera.rotation.x += recoil * 0.001
    }

    // Hide muzzle flash
    if (muzzleFlashRef.current) {
      muzzleFlashRef.current.visible = false
    }
  })

  // Shooting
  useEffect(() => {
    const handleShoot = (e) => {
      if (e.button === 0 && !isReloading && currentAmmo > 0) {
        shoot()

        // Muzzle flash
        if (muzzleFlashRef.current) {
          muzzleFlashRef.current.visible = true
        }

        // Recoil
        setRecoil(1)
        camera.rotation.x -= 0.02
        camera.rotation.y += (Math.random() - 0.5) * 0.01

        // Raycast for hit detection
        raycaster.setFromCamera({ x: 0, y: 0 }, camera)
        const intersects = raycaster.intersectObjects(scene.children, true)

        if (intersects.length > 0) {
          const hit = intersects[0]

          // Check if we hit a zombie
          let zombieHit = hit.object
          while (zombieHit && !zombieHit.userData.type) {
            zombieHit = zombieHit.parent
          }

          if (zombieHit && zombieHit.userData.type === 'zombie') {
            // Damage zombie via event
            const damageEvent = new CustomEvent('zombieDamage', {
              detail: {
                zombieId: zombieHit.userData.id,
                damage: 35 // M4A1 damage
              }
            })
            window.dispatchEvent(damageEvent)

            // Blood splatter effect
            const blood = new THREE.Mesh(
              new THREE.CircleGeometry(0.1, 8),
              new THREE.MeshBasicMaterial({ color: 0x8b0000 })
            )
            blood.position.copy(hit.point)
            blood.lookAt(hit.point.clone().add(hit.normal))
            scene.add(blood)
            setTimeout(() => scene.remove(blood), 3000)
          } else {
            // Create bullet hole on walls
            const bulletHole = new THREE.Mesh(
              new THREE.CircleGeometry(0.05, 8),
              new THREE.MeshBasicMaterial({ color: 0x222222 })
            )
            bulletHole.position.copy(hit.point)
            bulletHole.lookAt(hit.point.clone().add(hit.normal))
            scene.add(bulletHole)

            setTimeout(() => scene.remove(bulletHole), 5000)
          }
        }
      }
    }

    const handleReload = (e) => {
      if (e.code === 'KeyR') {
        reload()
      }
    }

    window.addEventListener('mousedown', handleShoot)
    window.addEventListener('keydown', handleReload)

    return () => {
      window.removeEventListener('mousedown', handleShoot)
      window.removeEventListener('keydown', handleReload)
    }
  }, [camera, raycaster, scene, shoot, reload, currentAmmo, isReloading])

  return (
    <group ref={weaponRef}>
      {/* M4A1 Body */}
      <mesh position={[0, 0, -0.2]} castShadow>
        <boxGeometry args={[0.08, 0.12, 0.6]} />
        <meshStandardMaterial
          color="#1a1a1a"
          metalness={0.7}
          roughness={0.3}
        />
      </mesh>

      {/* Magazine */}
      <mesh position={[0, -0.15, -0.1]} castShadow>
        <boxGeometry args={[0.06, 0.2, 0.15]} />
        <meshStandardMaterial
          color="#2a2a2a"
          metalness={0.6}
          roughness={0.4}
        />
      </mesh>

      {/* Barrel */}
      <mesh position={[0, 0.02, -0.6]} castShadow>
        <cylinderGeometry args={[0.015, 0.015, 0.4, 12]} rotation={[Math.PI / 2, 0, 0]} />
        <meshStandardMaterial
          color="#0a0a0a"
          metalness={0.9}
          roughness={0.2}
        />
      </mesh>

      {/* Front sight */}
      <mesh position={[0, 0.08, -0.7]} castShadow>
        <boxGeometry args={[0.02, 0.04, 0.02]} />
        <meshStandardMaterial color="#222" metalness={0.5} />
      </mesh>

      {/* Stock */}
      <mesh position={[0, -0.02, 0.15]} castShadow>
        <boxGeometry args={[0.06, 0.08, 0.3]} />
        <meshStandardMaterial
          color="#2a2a2a"
          metalness={0.4}
          roughness={0.6}
        />
      </mesh>

      {/* Grip */}
      <mesh position={[0, -0.08, -0.05]} castShadow rotation={[0.3, 0, 0]}>
        <boxGeometry args={[0.05, 0.12, 0.05]} />
        <meshStandardMaterial color="#1a1a1a" roughness={0.9} />
      </mesh>

      {/* Muzzle Flash - Enhanced */}
      <sprite ref={muzzleFlashRef} position={[0, 0, -0.82]} scale={[0.3, 0.3, 1]}>
        <spriteMaterial
          color="#ffaa00"
          transparent
          opacity={0.9}
          blending={2}
        />
      </sprite>

      {/* Point light for muzzle flash */}
      <pointLight
        ref={muzzleFlashRef}
        position={[0, 0, -0.8]}
        color="#ffaa00"
        intensity={0}
        distance={10}
      />
    </group>
  )
}

export default Weapon
