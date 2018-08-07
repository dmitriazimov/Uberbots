#include "Logic/StaticFunctionLibrary.h"

FVector StaticFunctionLibrary::GetAveragePlayerPosition(UWorld * world_context)
{
	float player_count = 0.0f;
	FVector player_location_sum = FVector(0, 0, 0);
	for (TActorIterator<AUberbotsCharacter> player_iter(world_context); player_iter; ++player_iter)
	{
		player_count++;
		player_location_sum += player_iter->GetActorLocation();
	}
	if (player_count > 0.0f)
		return player_location_sum / player_count;
	else
		return FVector();
}

FVector StaticFunctionLibrary::GetAveragePlayerPositionExceptFor(UWorld * world_context, AUberbotsCharacter * player_character)
{
	float player_count = 0.0f;
	FVector player_location_sum = FVector(0, 0, 0);
	if (Cast<AUberbotsCharacter>(player_character))
	{
		for (TActorIterator<AUberbotsCharacter> player_iter(world_context); player_iter; ++player_iter)
		{
			if ((*player_iter) != Cast<AUberbotsCharacter>(player_character))
			{
				player_count++;
				player_location_sum += player_iter->GetActorLocation();
			}
		}
		if (player_count > 0.0f)
			return player_location_sum / player_count;
		else
			return FVector();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("character cast failed"));
		return FVector();
	}
}

float StaticFunctionLibrary::FindLeftMostPlayerProgress(UWorld * world_context)
{
	float left_most_player_y = -TNumericLimits< float >::Max();
	for (TActorIterator<AUberbotsCharacter> living_characters_iter(world_context); living_characters_iter; ++living_characters_iter)
	{
		left_most_player_y = left_most_player_y < living_characters_iter->GetActorLocation().Y ? living_characters_iter->GetActorLocation().Y : left_most_player_y;
	}
	return left_most_player_y != -TNumericLimits< float >::Max() ? left_most_player_y : 0.0f;
}


void StaticFunctionLibrary::ConstrainCharactersToAxisProgress(UWorld * world_context, FVector progress)
{
	if (progress.Y != -1)
	{
		for (TActorIterator<AUberbotsCharacter> living_characters_iter(world_context); living_characters_iter; ++living_characters_iter)
		{
			if (living_characters_iter->GetActorLocation().Y > progress.Y)
			{
				living_characters_iter->SetActorLocation(FVector(living_characters_iter->GetActorLocation().X, progress.Y, living_characters_iter->GetActorLocation().Z));
			}
		}
	}
	else if (progress.Z != -1)
	{
		for (TActorIterator<AUberbotsCharacter> living_characters_iter(world_context); living_characters_iter; ++living_characters_iter)
		{
			if (living_characters_iter->GetActorLocation().Z < progress.Z)
			{
				living_characters_iter->SetActorLocation(FVector(living_characters_iter->GetActorLocation().X, living_characters_iter->GetActorLocation().Y, progress.Z));
			}
		}
	}
	else if (progress.X != -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempting to add depth movement to character"));
	}
}

AUberbotsCharacter* StaticFunctionLibrary::FindClosestPlayerCharacter(UWorld * world_context, AActor * to)
{
	float shortest_distance = TNumericLimits< float >::Max();
	AUberbotsCharacter* uberbot = nullptr;
	for (TActorIterator<AUberbotsCharacter> living_characters_iter(world_context); living_characters_iter; ++living_characters_iter)
	{
		float temp_distance = (living_characters_iter->GetActorLocation() - to->GetActorLocation()).Size();
		if (temp_distance < shortest_distance)
		{
			uberbot = *living_characters_iter;
			shortest_distance = temp_distance;
		}
	}
	return uberbot;
}

bool StaticFunctionLibrary::InDirectRange(UWorld * world_context, ACharacter * origin, ACharacter * target, float range)
{
	return FMath::Abs((origin->GetActorLocation() - target->GetActorLocation()).Size()) <= range;
}

int StaticFunctionLibrary::InRelativeRange(UWorld * world_context, ACharacter * origin, ACharacter * target, float range_min, float range_max)
{
	float distance = FMath::Abs((origin->GetActorLocation() - target->GetActorLocation()).Size());

	if (distance > range_max)
	{
		return 1;
	}
	else if (distance < range_min)
	{
		return -1;
	}
	return 0;
}

void StaticFunctionLibrary::Follow(AActor* follower, FVector destination, float speed)
{
	follower->SetActorLocation(FMath::Lerp(follower->GetActorLocation(), destination, speed));
}

FVector StaticFunctionLibrary::Circle(float radius, float seed, float y, float z)
{
	return FVector(0.0f, y + FMath::Cos(seed) * radius, z + FMath::Sin(seed) * radius);
}

StaticFunctionLibrary::StaticFunctionLibrary()
{
}

StaticFunctionLibrary::~StaticFunctionLibrary()
{
}
