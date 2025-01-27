#pragma once
UENUM(BlueprintType)
enum class EWeaponType : uint8
{

	EWT_OneHandWeapon UMETA(DisplayName = "One Hand Weapon"),
	EWT_TwoHandWeapon UMETA(DisplayName = "Two Hand Weapon")

};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped				UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOnehandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwohandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied				UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking				UMETA(DisplayName = "Attacking"),
	EAS_Equipping				UMETA(DisplayName = "Equipping"),
	EAS_HitReacting             UMETA(DisplayName = "Hit Reacting"),
	EAS_Dodge					UMETA(DisplayName = "Dodge"),
	EAS_Dead					UMETA(Displayname = "Dead")

};

UENUM(BlueprintType)
enum EDeathPose
{
	EDP_Death1 				    UMETA(DisplayName = "Death1"),
	EDP_Death2 				    UMETA(DisplayName = "Death2"),
	EDP_Death3 				    UMETA(DisplayName = "Death3"),
	EDP_Death4 				    UMETA(DisplayName = "Death4"),
	EDP_Death5 				    UMETA(DisplayName = "Death5"),

	EDP_MAX						UMETA(DisplayName = "DefaultMAX")

};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{	
	EES_NoState						UMETA(DisplayName = "NoState"),
	EES_Dead						UMETA(DisplayName = "Dead"),
	EES_Patrolling					UMETA(DisplayName = "Patrolling"),
	EES_Chasing						UMETA(DisplayName = "Chasing"),
	EES_Attacking				    UMETA(DisplayName = "Attacking"),
	EES_Engaged						UMETA(DisplayName = "Engaged")

};

