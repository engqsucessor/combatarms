using UnityEngine;
using UnityEngine.Events;

namespace CombatArms.Player
{
    public class PlayerHealth : MonoBehaviour
    {
        [Header("Health Settings")]
        [SerializeField] private float maxHealth = 100f;
        [SerializeField] private float maxArmor = 100f;
        [SerializeField] private float armorAbsorption = 0.5f; // 50% damage absorbed by armor

        [Header("Damage Multipliers")]
        [SerializeField] private float headShotMultiplier = 2f;
        [SerializeField] private float legMultiplier = 0.75f;
        [SerializeField] private float armMultiplier = 0.85f;

        private float currentHealth;
        private float currentArmor;

        public float CurrentHealth => currentHealth;
        public float MaxHealth => maxHealth;
        public float CurrentArmor => currentArmor;
        public float MaxArmor => maxArmor;
        public bool IsAlive => currentHealth > 0;

        public UnityEvent<float> OnHealthChanged;
        public UnityEvent<float> OnArmorChanged;
        public UnityEvent OnDeath;

        private void Awake()
        {
            currentHealth = maxHealth;
            currentArmor = maxArmor;
        }

        public void TakeDamage(float damage, HitLocation hitLocation = HitLocation.Body)
        {
            if (!IsAlive) return;

            // Apply hit location multiplier
            float multipliedDamage = damage * GetLocationMultiplier(hitLocation);

            // Calculate armor absorption
            if (currentArmor > 0)
            {
                float armorDamage = multipliedDamage * armorAbsorption;
                float healthDamage = multipliedDamage * (1f - armorAbsorption);

                currentArmor -= armorDamage;

                if (currentArmor < 0)
                {
                    // Overflow damage goes to health
                    healthDamage += Mathf.Abs(currentArmor);
                    currentArmor = 0;
                }

                currentHealth -= healthDamage;
                OnArmorChanged?.Invoke(currentArmor);
            }
            else
            {
                currentHealth -= multipliedDamage;
            }

            currentHealth = Mathf.Max(0, currentHealth);
            OnHealthChanged?.Invoke(currentHealth);

            if (currentHealth <= 0)
            {
                Die();
            }
        }

        public void Heal(float amount)
        {
            if (!IsAlive) return;

            currentHealth += amount;
            currentHealth = Mathf.Min(currentHealth, maxHealth);
            OnHealthChanged?.Invoke(currentHealth);
        }

        public void AddArmor(float amount)
        {
            currentArmor += amount;
            currentArmor = Mathf.Min(currentArmor, maxArmor);
            OnArmorChanged?.Invoke(currentArmor);
        }

        private float GetLocationMultiplier(HitLocation location)
        {
            switch (location)
            {
                case HitLocation.Head:
                    return headShotMultiplier;
                case HitLocation.Leg:
                    return legMultiplier;
                case HitLocation.Arm:
                    return armMultiplier;
                default:
                    return 1f;
            }
        }

        private void Die()
        {
            OnDeath?.Invoke();
            Debug.Log("Player died!");
        }

        public void Respawn()
        {
            currentHealth = maxHealth;
            currentArmor = maxArmor;
            OnHealthChanged?.Invoke(currentHealth);
            OnArmorChanged?.Invoke(currentArmor);
        }
    }

    public enum HitLocation
    {
        Head,
        Body,
        Arm,
        Leg
    }
}
