using UnityEngine;
using UnityEngine.UI;
using TMPro;

namespace CombatArms.UI
{
    public class GameHUD : MonoBehaviour
    {
        [Header("Health/Armor")]
        [SerializeField] private Image healthBar;
        [SerializeField] private Image armorBar;
        [SerializeField] private TextMeshProUGUI healthText;
        [SerializeField] private TextMeshProUGUI armorText;

        [Header("Ammo")]
        [SerializeField] private TextMeshProUGUI ammoText;
        [SerializeField] private TextMeshProUGUI reserveAmmoText;

        [Header("Stamina")]
        [SerializeField] private Image staminaBar;

        [Header("Wave Info")]
        [SerializeField] private TextMeshProUGUI waveText;
        [SerializeField] private TextMeshProUGUI zombiesText;

        [Header("Score")]
        [SerializeField] private TextMeshProUGUI scoreText;

        [Header("Crosshair")]
        [SerializeField] private Image crosshair;

        private Player.PlayerHealth playerHealth;
        private Player.FPSController playerController;
        private Weapons.WeaponBase currentWeapon;
        private GameMode.CabinFeverGameMode gameMode;

        private void Start()
        {
            // Find player components
            playerHealth = FindObjectOfType<Player.PlayerHealth>();
            playerController = FindObjectOfType<Player.FPSController>();
            gameMode = FindObjectOfType<GameMode.CabinFeverGameMode>();

            // Subscribe to events
            if (playerHealth != null)
            {
                playerHealth.OnHealthChanged.AddListener(UpdateHealth);
                playerHealth.OnArmorChanged.AddListener(UpdateArmor);
            }
        }

        private void Update()
        {
            UpdateStamina();
            UpdateWaveInfo();
            UpdateAmmo();
        }

        private void UpdateHealth(float currentHealth)
        {
            if (healthBar != null)
            {
                healthBar.fillAmount = currentHealth / playerHealth.MaxHealth;
            }
            if (healthText != null)
            {
                healthText.text = $"HP: {Mathf.RoundToInt(currentHealth)}";
            }
        }

        private void UpdateArmor(float currentArmor)
        {
            if (armorBar != null)
            {
                armorBar.fillAmount = currentArmor / playerHealth.MaxArmor;
            }
            if (armorText != null)
            {
                armorText.text = $"AP: {Mathf.RoundToInt(currentArmor)}";
            }
        }

        private void UpdateStamina()
        {
            if (playerController != null && staminaBar != null)
            {
                staminaBar.fillAmount = playerController.CurrentStamina / playerController.MaxStamina;
            }
        }

        private void UpdateWaveInfo()
        {
            if (gameMode != null)
            {
                if (waveText != null)
                {
                    waveText.text = $"Wave: {gameMode.CurrentWave}";
                }
                if (zombiesText != null)
                {
                    zombiesText.text = $"Zombies: {gameMode.ZombiesRemaining}";
                }
            }
        }

        private void UpdateAmmo()
        {
            currentWeapon = FindObjectOfType<Weapons.WeaponBase>();

            if (currentWeapon != null)
            {
                if (ammoText != null)
                {
                    ammoText.text = currentWeapon.CurrentAmmo.ToString();
                }
                if (reserveAmmoText != null)
                {
                    reserveAmmoText.text = $"/ {currentWeapon.ReserveAmmo}";
                }
            }
        }

        public void SetCrosshairColor(Color color)
        {
            if (crosshair != null)
            {
                crosshair.color = color;
            }
        }
    }
}
