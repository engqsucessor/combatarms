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

          // Check if we hit an enemy
          if (hit.object.userData.type === 'zombie') {
            // Deal damage (handled in enemy component)
            console.log('Hit zombie at', hit.point)
          }

          // Create bullet hole
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
      {/* Simple weapon model - will be replaced with actual model */}
      <mesh position={[0, 0, 0]}>
        <boxGeometry args={[0.1, 0.1, 0.8]} />
        <meshStandardMaterial color="#333" />
      </mesh>

      {/* Barrel */}
      <mesh position={[0, 0, -0.5]}>
        <cylinderGeometry args={[0.02, 0.02, 0.3]} />
        <meshStandardMaterial color="#111" />
      </mesh>

      {/* Muzzle Flash */}
      <sprite ref={muzzleFlashRef} position={[0, 0, -0.65]} scale={[0.2, 0.2, 1]}>
        <spriteMaterial color="#ffff00" transparent opacity={0.8} />
      </sprite>
    </group>
  )
}

export default Weapon
