using UnityEngine;
using UnityEngine.Events;

namespace CombatArms.Zombie
{
    public class ZombieHealth : MonoBehaviour
    {
        [Header("Health")]
        [SerializeField] private float maxHealth = 100f;
        [SerializeField] private int scoreValue = 100;

        private float currentHealth;
        public bool IsAlive => currentHealth > 0;

        public UnityEvent<float> OnHealthChanged;
        public UnityEvent<int> OnDeath; // Passes score value

        private void Awake()
        {
            currentHealth = maxHealth;
        }

        public void TakeDamage(float damage)
        {
            if (!IsAlive) return;

            currentHealth -= damage;
            currentHealth = Mathf.Max(0, currentHealth);

            OnHealthChanged?.Invoke(currentHealth);

            if (currentHealth <= 0)
            {
                Die();
            }
        }

        private void Die()
        {
            OnDeath?.Invoke(scoreValue);
            // Play death animation/sound
            Destroy(gameObject, 2f); // Destroy after animation
        }

        public void SetMaxHealth(float health)
        {
            maxHealth = health;
            currentHealth = health;
        }
    }
}
