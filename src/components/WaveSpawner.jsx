import { useState, useEffect, useRef } from 'react'
import { useGameStore } from '../stores/gameStore'
import Zombie from './Zombie'

function WaveSpawner({ playerRef }) {
  const [zombies, setZombies] = useState([])
  const [nextZombieId, setNextZombieId] = useState(0)
  const { wave, zombiesRemaining, gamePhase, startWave, completeWave } = useGameStore()
  const waveTimerRef = useRef(null)

  // Spawn points around the map
  const spawnPoints = [
    [-15, 1, -15],
    [15, 1, -15],
    [-15, 1, 15],
    [15, 1, 15],
    [0, 1, -20],
    [0, 1, 20],
    [-20, 1, 0],
    [20, 1, 0],
  ]

  // Start wave system
  useEffect(() => {
    if (gamePhase === 'preparing') {
      // Wait 5 seconds before starting first wave
      waveTimerRef.current = setTimeout(() => {
        startWave(1)
        spawnWave(1)
      }, 5000)
    }

    return () => {
      if (waveTimerRef.current) {
        clearTimeout(waveTimerRef.current)
      }
    }
  }, [])

  // Check wave completion
  useEffect(() => {
    if (gamePhase === 'combat' && zombiesRemaining === 0 && zombies.length === 0) {
      completeWave()

      // Wait 10 seconds then start next wave
      setTimeout(() => {
        const nextWave = wave + 1
        startWave(nextWave)
        spawnWave(nextWave)
      }, 10000)
    }
  }, [zombiesRemaining, zombies.length, gamePhase, wave])

  // Spawn zombies for a wave
  const spawnWave = (waveNumber) => {
    const zombieCount = 5 + (waveNumber * 3) // Increase per wave
    const newZombies = []

    // Spawn zombies over time
    for (let i = 0; i < zombieCount; i++) {
      setTimeout(() => {
        const spawnPoint = spawnPoints[Math.floor(Math.random() * spawnPoints.length)]
        const zombieId = nextZombieId + i

        setZombies(prev => [...prev, {
          id: zombieId,
          position: [...spawnPoint]
        }])
      }, i * 1000) // 1 second between each spawn
    }

    setNextZombieId(prev => prev + zombieCount)
  }

  // Remove dead zombies
  useEffect(() => {
    const handleZombieDeath = (event) => {
      setZombies(prev => prev.filter(z => z.id !== event.detail.zombieId))
    }

    window.addEventListener('zombieDeath', handleZombieDeath)
    return () => window.removeEventListener('zombieDeath', handleZombieDeath)
  }, [])

  return (
    <>
      {zombies.map(zombie => (
        <Zombie
          key={zombie.id}
          id={zombie.id}
          position={zombie.position}
          playerRef={playerRef}
        />
      ))}
    </>
  )
}

export default WaveSpawner
