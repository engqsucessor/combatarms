import { useRef, useEffect, useState, forwardRef, useImperativeHandle } from 'react'
import { useFrame, useThree } from '@react-three/fiber'
import { RigidBody, CapsuleCollider } from '@react-three/rapier'
import { useKeyboardControls } from '../hooks/useKeyboardControls'
import { Vector3 } from 'three'
import Weapon from './Weapon'

const MOVE_SPEED = 5
const SPRINT_MULTIPLIER = 1.5
const CROUCH_MULTIPLIER = 0.5
const JUMP_FORCE = 8

const Player = forwardRef((props, ref) => {
  const playerRef = useRef()

  // Expose playerRef to parent
  useImperativeHandle(ref, () => playerRef.current)
  const { camera } = useThree()
  const [velocity] = useState(() => new Vector3())
  const keys = useKeyboardControls()

  const [isSprinting, setIsSprinting] = useState(false)
  const [isCrouching, setIsCrouching] = useState(false)

  // Position camera at player position
  useFrame(() => {
    if (!playerRef.current) return

    const position = playerRef.current.translation()
    const yOffset = isCrouching ? 0.5 : 1.6
    camera.position.set(position.x, position.y + yOffset, position.z)
  })

  // Movement
  useFrame((state, delta) => {
    if (!playerRef.current) return

    const currentVel = playerRef.current.linvel()

    // Calculate movement direction
    const forward = new Vector3(0, 0, -1).applyQuaternion(camera.quaternion)
    const right = new Vector3(1, 0, 0).applyQuaternion(camera.quaternion)

    forward.y = 0
    right.y = 0
    forward.normalize()
    right.normalize()

    velocity.set(0, currentVel.y, 0)

    // Apply movement
    if (keys.forward) velocity.add(forward.multiplyScalar(MOVE_SPEED))
    if (keys.backward) velocity.add(forward.multiplyScalar(-MOVE_SPEED))
    if (keys.left) velocity.add(right.multiplyScalar(-MOVE_SPEED))
    if (keys.right) velocity.add(right.multiplyScalar(MOVE_SPEED))

    // Sprint/crouch
    const speedMultiplier = isSprinting ? SPRINT_MULTIPLIER : (isCrouching ? CROUCH_MULTIPLIER : 1)
    velocity.x *= speedMultiplier
    velocity.z *= speedMultiplier

    playerRef.current.setLinvel(velocity, true)
  })

  // Jump
  useEffect(() => {
    const handleJump = (e) => {
      if (e.code === 'Space' && playerRef.current) {
        const currentVel = playerRef.current.linvel()
        playerRef.current.setLinvel({ x: currentVel.x, y: JUMP_FORCE, z: currentVel.z }, true)
      }
      if (e.code === 'ShiftLeft') setIsSprinting(true)
      if (e.code === 'ControlLeft') setIsCrouching(true)
    }

    const handleKeyUp = (e) => {
      if (e.code === 'ShiftLeft') setIsSprinting(false)
      if (e.code === 'ControlLeft') setIsCrouching(false)
    }

    window.addEventListener('keydown', handleJump)
    window.addEventListener('keyup', handleKeyUp)

    return () => {
      window.removeEventListener('keydown', handleJump)
      window.removeEventListener('keyup', handleKeyUp)
    }
  }, [])

  return (
    <>
      <RigidBody
        ref={playerRef}
        position={[0, 5, 0]}
        enabledRotations={[false, false, false]}
        mass={80}
        lockRotations
      >
        <CapsuleCollider args={[0.8, 0.4]} />
      </RigidBody>

      <Weapon />
    </>
  )
})

export default Player
