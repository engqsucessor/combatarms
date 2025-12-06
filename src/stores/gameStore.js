import { create } from 'zustand'

export const useGameStore = create((set) => ({
  // Player stats
  health: 100,
  armor: 100,
  stamina: 100,

  // Weapon state
  currentWeapon: 'M4A1',
  currentAmmo: 30,
  reserveAmmo: 90,
  isReloading: false,

  // Game state
  wave: 1,
  score: 0,
  kills: 0,
  zombiesRemaining: 0,
  gamePhase: 'preparing', // preparing, combat, complete

  // Actions
  takeDamage: (amount) => set((state) => {
    let newHealth = state.health
    let newArmor = state.armor

    if (newArmor > 0) {
      const armorDamage = amount * 0.5
      const healthDamage = amount * 0.5

      newArmor = Math.max(0, newArmor - armorDamage)
      newHealth = Math.max(0, newHealth - healthDamage)
    } else {
      newHealth = Math.max(0, newHealth - amount)
    }

    return { health: newHealth, armor: newArmor }
  }),

  shoot: () => set((state) => {
    if (state.isReloading || state.currentAmmo <= 0) return state

    return { currentAmmo: state.currentAmmo - 1 }
  }),

  reload: () => set((state) => {
    if (state.isReloading || state.currentAmmo === 30 || state.reserveAmmo === 0) return state

    setTimeout(() => {
      set((s) => {
        const ammoNeeded = 30 - s.currentAmmo
        const ammoToReload = Math.min(ammoNeeded, s.reserveAmmo)

        return {
          currentAmmo: s.currentAmmo + ammoToReload,
          reserveAmmo: s.reserveAmmo - ammoToReload,
          isReloading: false
        }
      })
    }, 2000)

    return { isReloading: true }
  }),

  addKill: () => set((state) => ({
    kills: state.kills + 1,
    score: state.score + 100,
    zombiesRemaining: state.zombiesRemaining - 1
  })),

  startWave: (waveNumber) => set(() => ({
    wave: waveNumber,
    zombiesRemaining: 5 + (waveNumber * 3),
    gamePhase: 'combat'
  })),

  completeWave: () => set((state) => ({
    gamePhase: 'complete',
    score: state.score + 500
  }))
}))
