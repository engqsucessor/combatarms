using UnityEngine;
using UnityEngine.AI;

namespace CombatArms.Zombie
{
    [RequireComponent(typeof(NavMeshAgent))]
    [RequireComponent(typeof(ZombieHealth))]
    public class ZombieAI : MonoBehaviour
    {
        [Header("Movement")]
        [SerializeField] private float moveSpeed = 3.5f;
        [SerializeField] private float chaseSpeedMultiplier = 1.5f;

        [Header("Attack")]
        [SerializeField] private float attackRange = 2f;
        [SerializeField] private float attackDamage = 10f;
        [SerializeField] private float attackCooldown = 1f;

        [Header("Detection")]
        [SerializeField] private float detectionRange = 15f;
        [SerializeField] private LayerMask playerLayer;

        private NavMeshAgent agent;
        private ZombieHealth health;
        private Transform currentTarget;
        private float lastAttackTime;
        private ZombieState currentState = ZombieState.Idle;

        private void Awake()
        {
            agent = GetComponent<NavMeshAgent>();
            health = GetComponent<ZombieHealth>();
            agent.speed = moveSpeed;
        }

        private void Update()
        {
            if (!health.IsAlive)
            {
                agent.isStopped = true;
                return;
            }

            FindTarget();
            UpdateBehavior();
        }

        private void FindTarget()
        {
            // Find nearest player
            Collider[] playersInRange = Physics.OverlapSphere(transform.position, detectionRange, playerLayer);

            if (playersInRange.Length > 0)
            {
                float closestDistance = Mathf.Infinity;
                Transform closestPlayer = null;

                foreach (var playerCollider in playersInRange)
                {
                    float distance = Vector3.Distance(transform.position, playerCollider.transform.position);
                    if (distance < closestDistance)
                    {
                        closestDistance = distance;
                        closestPlayer = playerCollider.transform;
                    }
                }

                currentTarget = closestPlayer;
            }
        }

        private void UpdateBehavior()
        {
            if (currentTarget == null)
            {
                currentState = ZombieState.Idle;
                agent.isStopped = true;
                return;
            }

            float distanceToTarget = Vector3.Distance(transform.position, currentTarget.position);

            if (distanceToTarget <= attackRange)
            {
                // Attack
                currentState = ZombieState.Attacking;
                agent.isStopped = true;
                AttackTarget();
            }
            else if (distanceToTarget <= detectionRange)
            {
                // Chase
                currentState = ZombieState.Chasing;
                agent.isStopped = false;
                agent.speed = moveSpeed * chaseSpeedMultiplier;
                agent.SetDestination(currentTarget.position);
            }
            else
            {
                currentState = ZombieState.Idle;
                agent.isStopped = true;
            }
        }

        private void AttackTarget()
        {
            // Face target
            Vector3 direction = (currentTarget.position - transform.position).normalized;
            Quaternion lookRotation = Quaternion.LookRotation(new Vector3(direction.x, 0, direction.z));
            transform.rotation = Quaternion.Slerp(transform.rotation, lookRotation, Time.deltaTime * 5f);

            // Attack if cooldown elapsed
            if (Time.time >= lastAttackTime + attackCooldown)
            {
                var playerHealth = currentTarget.GetComponent<Player.PlayerHealth>();
                if (playerHealth != null)
                {
                    playerHealth.TakeDamage(attackDamage);
                    lastAttackTime = Time.time;
                    Debug.Log("Zombie attacked player!");
                }
            }
        }

        private void OnDrawGizmosSelected()
        {
            // Detection range
            Gizmos.color = Color.yellow;
            Gizmos.DrawWireSphere(transform.position, detectionRange);

            // Attack range
            Gizmos.color = Color.red;
            Gizmos.DrawWireSphere(transform.position, attackRange);
        }
    }

    public enum ZombieState
    {
        Idle,
        Chasing,
        Attacking
    }
}
