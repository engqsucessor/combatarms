using UnityEngine;

namespace CombatArms.Player
{
    [RequireComponent(typeof(CharacterController))]
    public class FPSController : MonoBehaviour
    {
        [Header("Movement")]
        [SerializeField] private float walkSpeed = 5f;
        [SerializeField] private float sprintSpeed = 8f;
        [SerializeField] private float crouchSpeed = 2.5f;
        [SerializeField] private float jumpHeight = 1.5f;
        [SerializeField] private float gravity = -19.62f;

        [Header("Mouse Look")]
        [SerializeField] private float mouseSensitivity = 2f;
        [SerializeField] private float maxLookAngle = 80f;
        [SerializeField] private Transform cameraTransform;

        [Header("Stamina")]
        [SerializeField] private float maxStamina = 100f;
        [SerializeField] private float sprintStaminaDrain = 20f;
        [SerializeField] private float staminaRegenRate = 15f;

        [Header("Ground Check")]
        [SerializeField] private float groundCheckDistance = 0.3f;
        [SerializeField] private LayerMask groundMask;

        private CharacterController controller;
        private Vector3 velocity;
        private float currentStamina;
        private float rotationX = 0f;
        private bool isGrounded;
        private bool isSprinting;
        private bool isCrouching;

        public float CurrentStamina => currentStamina;
        public float MaxStamina => maxStamina;
        public bool IsSprinting => isSprinting;

        private void Awake()
        {
            controller = GetComponent<CharacterController>();
            currentStamina = maxStamina;

            // Lock cursor
            Cursor.lockState = CursorLockMode.Locked;
            Cursor.visible = false;

            if (cameraTransform == null)
            {
                cameraTransform = Camera.main.transform;
            }
        }

        private void Update()
        {
            HandleMouseLook();
            HandleMovement();
            HandleStamina();
        }

        private void HandleMouseLook()
        {
            float mouseX = Input.GetAxis("Mouse X") * mouseSensitivity;
            float mouseY = Input.GetAxis("Mouse Y") * mouseSensitivity;

            // Rotate player horizontally
            transform.Rotate(Vector3.up * mouseX);

            // Rotate camera vertically
            rotationX -= mouseY;
            rotationX = Mathf.Clamp(rotationX, -maxLookAngle, maxLookAngle);
            cameraTransform.localRotation = Quaternion.Euler(rotationX, 0f, 0f);
        }

        private void HandleMovement()
        {
            // Ground check
            isGrounded = Physics.Raycast(transform.position, Vector3.down, groundCheckDistance, groundMask);

            if (isGrounded && velocity.y < 0)
            {
                velocity.y = -2f;
            }

            // Get input
            float x = Input.GetAxis("Horizontal");
            float z = Input.GetAxis("Vertical");

            // Movement
            Vector3 move = transform.right * x + transform.forward * z;

            // Sprint/Crouch
            isSprinting = Input.GetKey(KeyCode.LeftShift) && currentStamina > 0 && z > 0 && !isCrouching;
            isCrouching = Input.GetKey(KeyCode.LeftControl);

            float currentSpeed = isSprinting ? sprintSpeed : (isCrouching ? crouchSpeed : walkSpeed);
            controller.Move(move * currentSpeed * Time.deltaTime);

            // Jump
            if (Input.GetButtonDown("Jump") && isGrounded && !isCrouching)
            {
                velocity.y = Mathf.Sqrt(jumpHeight * -2f * gravity);
            }

            // Apply gravity
            velocity.y += gravity * Time.deltaTime;
            controller.Move(velocity * Time.deltaTime);
        }

        private void HandleStamina()
        {
            if (isSprinting)
            {
                currentStamina -= sprintStaminaDrain * Time.deltaTime;
                currentStamina = Mathf.Max(0, currentStamina);
            }
            else if (currentStamina < maxStamina)
            {
                currentStamina += staminaRegenRate * Time.deltaTime;
                currentStamina = Mathf.Min(maxStamina, currentStamina);
            }
        }

        private void OnDrawGizmosSelected()
        {
            // Draw ground check ray
            Gizmos.color = Color.red;
            Gizmos.DrawRay(transform.position, Vector3.down * groundCheckDistance);
        }
    }
}
