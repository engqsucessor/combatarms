using UnityEngine;
using UnityEngine.Events;

namespace CombatArms.Weapons
{
    public class WeaponBase : MonoBehaviour
    {
        [Header("Weapon Stats")]
        [SerializeField] protected string weaponName = "Weapon";
        [SerializeField] protected float damage = 30f;
        [SerializeField] protected float fireRate = 0.1f; // Time between shots
        [SerializeField] protected float range = 100f;
        [SerializeField] protected int magazineSize = 30;
        [SerializeField] protected int reserveAmmo = 120;
        [SerializeField] protected float reloadTime = 2f;

        [Header("Accuracy")]
        [SerializeField] protected float hipfireSpread = 0.1f;
        [SerializeField] protected float adsSpread = 0.02f;
        [SerializeField] protected float recoilVertical = 1f;
        [SerializeField] protected float recoilHorizontal = 0.5f;

        [Header("Fire Mode")]
        [SerializeField] protected FireMode fireMode = FireMode.Auto;

        [Header("Effects")]
        [SerializeField] protected ParticleSystem muzzleFlash;
        [SerializeField] protected Transform muzzlePoint;
        [SerializeField] protected AudioClip fireSound;
        [SerializeField] protected AudioClip reloadSound;

        protected int currentAmmo;
        protected float nextFireTime;
        protected bool isReloading;
        protected bool isAiming;

        public int CurrentAmmo => currentAmmo;
        public int ReserveAmmo => reserveAmmo;
        public bool IsReloading => isReloading;
        public string WeaponName => weaponName;

        public UnityEvent<int, int> OnAmmoChanged;
        public UnityEvent OnWeaponFired;
        public UnityEvent OnReloadStart;
        public UnityEvent OnReloadComplete;

        protected virtual void Awake()
        {
            currentAmmo = magazineSize;
        }

        protected virtual void Update()
        {
            HandleInput();
        }

        protected virtual void HandleInput()
        {
            // Aim
            isAiming = Input.GetButton("Fire2");

            // Fire
            bool fireInput = fireMode == FireMode.Auto ? Input.GetButton("Fire1") : Input.GetButtonDown("Fire1");

            if (fireInput && CanFire())
            {
                Fire();
            }

            // Reload
            if (Input.GetKeyDown(KeyCode.R) && CanReload())
            {
                StartReload();
            }
        }

        protected virtual bool CanFire()
        {
            return !isReloading && currentAmmo > 0 && Time.time >= nextFireTime;
        }

        protected virtual void Fire()
        {
            currentAmmo--;
            nextFireTime = Time.time + fireRate;

            // Raycast for hit detection
            Ray ray = Camera.main.ViewportPointToRay(new Vector3(0.5f, 0.5f, 0f));

            // Add spread
            float spread = isAiming ? adsSpread : hipfireSpread;
            Vector3 spreadDirection = ray.direction;
            spreadDirection.x += Random.Range(-spread, spread);
            spreadDirection.y += Random.Range(-spread, spread);
            ray.direction = spreadDirection.normalized;

            RaycastHit hit;
            if (Physics.Raycast(ray, out hit, range))
            {
                HandleHit(hit);
            }

            // Effects
            if (muzzleFlash != null)
            {
                muzzleFlash.Play();
            }

            // Recoil
            ApplyRecoil();

            OnWeaponFired?.Invoke();
            OnAmmoChanged?.Invoke(currentAmmo, reserveAmmo);

            // Auto reload when empty
            if (currentAmmo == 0 && reserveAmmo > 0)
            {
                StartReload();
            }
        }

        protected virtual void HandleHit(RaycastHit hit)
        {
            // Check if we hit a damageable object
            var health = hit.collider.GetComponent<Player.PlayerHealth>();
            if (health != null)
            {
                // Determine hit location
                Player.HitLocation hitLoc = DetermineHitLocation(hit.collider, hit.point);
                health.TakeDamage(damage, hitLoc);
            }

            var zombieHealth = hit.collider.GetComponent<Zombie.ZombieHealth>();
            if (zombieHealth != null)
            {
                zombieHealth.TakeDamage(damage);
            }

            // Spawn impact effect
            Debug.DrawRay(hit.point, hit.normal, Color.red, 2f);
        }

        protected virtual Player.HitLocation DetermineHitLocation(Collider hitCollider, Vector3 hitPoint)
        {
            // Simple height-based detection (improve with proper hitboxes later)
            float relativeHeight = (hitPoint.y - hitCollider.bounds.min.y) / hitCollider.bounds.size.y;

            if (relativeHeight > 0.8f)
                return Player.HitLocation.Head;
            else if (relativeHeight < 0.4f)
                return Player.HitLocation.Leg;
            else
                return Player.HitLocation.Body;
        }

        protected virtual void ApplyRecoil()
        {
            // Apply camera recoil
            float verticalRecoil = Random.Range(recoilVertical * 0.8f, recoilVertical * 1.2f);
            float horizontalRecoil = Random.Range(-recoilHorizontal, recoilHorizontal);

            Camera.main.transform.Rotate(-verticalRecoil, horizontalRecoil, 0f);
        }

        protected virtual bool CanReload()
        {
            return !isReloading && currentAmmo < magazineSize && reserveAmmo > 0;
        }

        protected virtual void StartReload()
        {
            isReloading = true;
            OnReloadStart?.Invoke();
            Invoke(nameof(CompleteReload), reloadTime);
        }

        protected virtual void CompleteReload()
        {
            int ammoNeeded = magazineSize - currentAmmo;
            int ammoToReload = Mathf.Min(ammoNeeded, reserveAmmo);

            currentAmmo += ammoToReload;
            reserveAmmo -= ammoToReload;

            isReloading = false;
            OnReloadComplete?.Invoke();
            OnAmmoChanged?.Invoke(currentAmmo, reserveAmmo);
        }
    }

    public enum FireMode
    {
        Auto,
        Semi,
        Burst
    }
}
