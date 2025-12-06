using UnityEngine;

namespace CombatArms.Weapons
{
    public class AssaultRifle : WeaponBase
    {
        public static AssaultRifle CreateM4A1(Transform parent)
        {
            GameObject weaponObj = new GameObject("M4A1");
            weaponObj.transform.SetParent(parent);
            weaponObj.transform.localPosition = new Vector3(0.3f, -0.3f, 0.5f);
            weaponObj.transform.localRotation = Quaternion.identity;

            AssaultRifle rifle = weaponObj.AddComponent<AssaultRifle>();
            rifle.weaponName = "M4A1";
            rifle.damage = 35f;
            rifle.fireRate = 0.08f; // 750 RPM
            rifle.range = 100f;
            rifle.magazineSize = 30;
            rifle.reserveAmmo = 180;
            rifle.reloadTime = 2.0f;
            rifle.hipfireSpread = 0.08f;
            rifle.adsSpread = 0.02f;
            rifle.recoilVertical = 0.5f;
            rifle.recoilHorizontal = 0.3f;
            rifle.fireMode = FireMode.Auto;

            return rifle;
        }

        public static AssaultRifle CreateAK47(Transform parent)
        {
            GameObject weaponObj = new GameObject("AK47");
            weaponObj.transform.SetParent(parent);
            weaponObj.transform.localPosition = new Vector3(0.3f, -0.3f, 0.5f);
            weaponObj.transform.localRotation = Quaternion.identity;

            AssaultRifle rifle = weaponObj.AddComponent<AssaultRifle>();
            rifle.weaponName = "AK-47";
            rifle.damage = 42f;
            rifle.fireRate = 0.1f; // 600 RPM
            rifle.range = 100f;
            rifle.magazineSize = 30;
            rifle.reserveAmmo = 180;
            rifle.reloadTime = 2.2f;
            rifle.hipfireSpread = 0.1f;
            rifle.adsSpread = 0.03f;
            rifle.recoilVertical = 0.8f;
            rifle.recoilHorizontal = 0.5f;
            rifle.fireMode = FireMode.Auto;

            return rifle;
        }

        public static AssaultRifle CreateSCARL(Transform parent)
        {
            GameObject weaponObj = new GameObject("SCAR-L");
            weaponObj.transform.SetParent(parent);
            weaponObj.transform.localPosition = new Vector3(0.3f, -0.3f, 0.5f);
            weaponObj.transform.localRotation = Quaternion.identity;

            AssaultRifle rifle = weaponObj.AddComponent<AssaultRifle>();
            rifle.weaponName = "SCAR-L";
            rifle.damage = 38f;
            rifle.fireRate = 0.092f; // 650 RPM
            rifle.range = 100f;
            rifle.magazineSize = 30;
            rifle.reserveAmmo = 180;
            rifle.reloadTime = 2.1f;
            rifle.hipfireSpread = 0.07f;
            rifle.adsSpread = 0.025f;
            rifle.recoilVertical = 0.6f;
            rifle.recoilHorizontal = 0.4f;
            rifle.fireMode = FireMode.Auto;

            return rifle;
        }
    }
}
