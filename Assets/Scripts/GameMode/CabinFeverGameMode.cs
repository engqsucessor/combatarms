using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace CombatArms.GameMode
{
    public class CabinFeverGameMode : MonoBehaviour
    {
        [Header("Wave Settings")]
        [SerializeField] private int startingWave = 1;
        [SerializeField] private float wavePrepTime = 30f;
        [SerializeField] private float timeBetweenSpawns = 2f;

        [Header("Zombie Spawning")]
        [SerializeField] private GameObject zombiePrefab;
        [SerializeField] private Transform[] spawnPoints;

        [Header("Difficulty Scaling")]
        [SerializeField] private int baseZombiesPerWave = 5;
        [SerializeField] private int zombiesPerWaveIncrease = 3;
        [SerializeField] private float healthScaling = 1.2f; // 20% more health per wave

        private int currentWave = 0;
        private int zombiesSpawned = 0;
        private int zombiesKilled = 0;
        private int totalZombiesThisWave = 0;
        private bool waveInProgress = false;
        private List<GameObject> activeZombies = new List<GameObject>();

        public int CurrentWave => currentWave;
        public int ZombiesRemaining => totalZombiesThisWave - zombiesKilled;

        private void Start()
        {
            StartCoroutine(GameLoop());
        }

        private IEnumerator GameLoop()
        {
            while (true)
            {
                currentWave++;
                yield return StartCoroutine(PrepareWave());
                yield return StartCoroutine(RunWave());
                yield return StartCoroutine(WaveComplete());
            }
        }

        private IEnumerator PrepareWave()
        {
            Debug.Log($"Preparing Wave {currentWave}...");
            waveInProgress = false;

            // Calculate zombies for this wave
            totalZombiesThisWave = baseZombiesPerWave + ((currentWave - 1) * zombiesPerWaveIncrease);
            zombiesSpawned = 0;
            zombiesKilled = 0;

            // Wait for prep time
            float timer = wavePrepTime;
            while (timer > 0)
            {
                // Display countdown UI here
                timer -= Time.deltaTime;
                yield return null;
            }
        }

        private IEnumerator RunWave()
        {
            Debug.Log($"Wave {currentWave} started! Zombies: {totalZombiesThisWave}");
            waveInProgress = true;

            // Spawn zombies over time
            while (zombiesSpawned < totalZombiesThisWave)
            {
                SpawnZombie();
                zombiesSpawned++;
                yield return new WaitForSeconds(timeBetweenSpawns);
            }

            // Wait until all zombies are killed
            while (zombiesKilled < totalZombiesThisWave)
            {
                // Clean up dead zombies
                activeZombies.RemoveAll(z => z == null);
                yield return null;
            }
        }

        private IEnumerator WaveComplete()
        {
            Debug.Log($"Wave {currentWave} complete!");
            waveInProgress = false;

            // Show wave complete UI
            yield return new WaitForSeconds(5f);
        }

        private void SpawnZombie()
        {
            if (spawnPoints.Length == 0)
            {
                Debug.LogWarning("No spawn points assigned!");
                return;
            }

            // Pick random spawn point
            Transform spawnPoint = spawnPoints[Random.Range(0, spawnPoints.length)];

            // Spawn zombie
            GameObject zombie = Instantiate(zombiePrefab, spawnPoint.position, spawnPoint.rotation);

            // Scale health based on wave
            var health = zombie.GetComponent<Zombie.ZombieHealth>();
            if (health != null)
            {
                float scaledHealth = 100f * Mathf.Pow(healthScaling, currentWave - 1);
                health.SetMaxHealth(scaledHealth);
            }

            // Listen to death event
            health.OnDeath.AddListener((score) => OnZombieKilled(zombie, score));

            activeZombies.Add(zombie);
        }

        private void OnZombieKilled(GameObject zombie, int score)
        {
            zombiesKilled++;
            // Add score to player here
            Debug.Log($"Zombie killed! Score: {score}. Remaining: {ZombiesRemaining}");
        }

        private void OnDrawGizmos()
        {
            if (spawnPoints == null) return;

            Gizmos.color = Color.red;
            foreach (var spawnPoint in spawnPoints)
            {
                if (spawnPoint != null)
                {
                    Gizmos.DrawWireSphere(spawnPoint.position, 0.5f);
                }
            }
        }
    }
}
