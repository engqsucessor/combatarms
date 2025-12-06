import { useState, useEffect } from 'react'

export function useKeyboardControls() {
  const [keys, setKeys] = useState({
    forward: false,
    backward: false,
    left: false,
    right: false,
    sprint: false,
    crouch: false
  })

  useEffect(() => {
    const handleKeyDown = (e) => {
      switch (e.code) {
        case 'KeyW':
        case 'ArrowUp':
          setKeys(k => ({ ...k, forward: true }))
          break
        case 'KeyS':
        case 'ArrowDown':
          setKeys(k => ({ ...k, backward: true }))
          break
        case 'KeyA':
        case 'ArrowLeft':
          setKeys(k => ({ ...k, left: true }))
          break
        case 'KeyD':
        case 'ArrowRight':
          setKeys(k => ({ ...k, right: true }))
          break
        case 'ShiftLeft':
          setKeys(k => ({ ...k, sprint: true }))
          break
        case 'ControlLeft':
          setKeys(k => ({ ...k, crouch: true }))
          break
      }
    }

    const handleKeyUp = (e) => {
      switch (e.code) {
        case 'KeyW':
        case 'ArrowUp':
          setKeys(k => ({ ...k, forward: false }))
          break
        case 'KeyS':
        case 'ArrowDown':
          setKeys(k => ({ ...k, backward: false }))
          break
        case 'KeyA':
        case 'ArrowLeft':
          setKeys(k => ({ ...k, left: false }))
          break
        case 'KeyD':
        case 'ArrowRight':
          setKeys(k => ({ ...k, right: false }))
          break
        case 'ShiftLeft':
          setKeys(k => ({ ...k, sprint: false }))
          break
        case 'ControlLeft':
          setKeys(k => ({ ...k, crouch: false }))
          break
      }
    }

    window.addEventListener('keydown', handleKeyDown)
    window.addEventListener('keyup', handleKeyUp)

    return () => {
      window.removeEventListener('keydown', handleKeyDown)
      window.removeEventListener('keyup', handleKeyUp)
    }
  }, [])

  return keys
}
