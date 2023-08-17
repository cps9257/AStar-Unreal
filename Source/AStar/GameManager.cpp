// Fill out your copyright notice in the Description page of Project Settings.

#include "GameManager.h"


// Sets default values
AGameManager::AGameManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	_map = CreateMap(mapSize);

	auto playController = GetWorld()->GetFirstPlayerController();
	AStarPlayerController = Cast<AAStarPlayerController>(playController);
	AStarPlayerController->OnMouseClicked.BindUObject(this, &AGameManager::FindPath);

	vector<AGridNode*> initPath{ _map[0] };
	AStarPlayerController->UpdatePath(initPath);
	AStarPlayerController->SetViewTargetWithBlend(camera);
}

// Called every frame
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

map<int, AGridNode*> AGameManager::CreateMap(FIntPoint mapSize)
{
	map<int, AGridNode*> map;

	_mapSize = mapSize;

	for (int i = 0;i < _mapSize.X;i++)
	{
		for (int j = 0;j < _mapSize.Y;j++)
		{
			FIntPoint coordinate = { i,j };
			int index = CoordinateToIndex(coordinate, _mapSize);
			bool isObstacle = IsObstacle(coordinate);
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			AGridNode* newNode = GetWorld()->SpawnActor<AGridNode>(gridNode, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
			newNode->Initialize(coordinate, isObstacle);
			newNode->Render(isObstacle ? GridNodeState::Obstacle : GridNodeState::None);
			FVector initPos = { i * newNode->GridSize.X, j * newNode->GridSize.Y, GRID_SIZE_Z };
			newNode->SetActorLocation(initPos);
			newNode->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

			map[index] = newNode;
		}
	}

	return map;
}

void AGameManager::ClearMap(map<int, AGridNode*> map)
{
	for (const auto& pair : map)
	{
		AGridNode * node = pair.second;
		node->Reset();
	}
}

void AGameManager::FindPath(FIntPoint startPos, FIntPoint targetPos)
{
	if (IsObstacle(targetPos))
		return;

	auto path = FindPath(_map, startPos, targetPos);

	AStarPlayerController->UpdatePath(path);
}

struct GridNodeComparison
{
	bool operator()(AGridNode* a, AGridNode * b) {
		return a->FScore() < b->FScore();
	}
};

vector<AGridNode*> AGameManager::FindPath(map<int, AGridNode*> map, FIntPoint startPos, FIntPoint targetPos)
{
	ClearMap(map);

	vector<AGridNode*> path;
	vector<AGridNode*> openSet; //List of nodes to be traversed

	FIntPoint directions[] = { {1,0},{0,1},{ -1,0 }, { 0,-1 } };
	int startIndex = CoordinateToIndex(startPos, _mapSize);
	int targetIndex = CoordinateToIndex(targetPos, _mapSize);

	AGridNode *startNode = nullptr;
	AGridNode *targetNode = nullptr;

	if (map.count(startIndex) > 0)
	{
		startNode = map[startIndex];
	}

	if (map.count(targetIndex) > 0)
	{
		targetNode = map[targetIndex];
	}

	startNode->GScore = 0;
	startNode->HScore = Distance(startNode->Coordinate, targetPos);
	startNode->Render(GridNodeState::Start);
	targetNode->Render(GridNodeState::Target);

	openSet.push_back(startNode);

	while (!openSet.empty())
	{
		sort(openSet.begin(), openSet.end(), GridNodeComparison());

		AGridNode *currentNode = openSet.front();

		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Orange, FString::FromInt(openSet.size()));

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("Current:") +  currentNode->ToString());*/

		if (currentNode->Coordinate == targetNode->Coordinate) {
			path = ReconstructPath(targetNode);
			break;
		}

		openSet.erase(openSet.begin());

		for (auto direction : directions)
		{
			FIntPoint neighborPos = currentNode->Coordinate + direction;

			if (neighborPos.X < 0 || neighborPos.X >= _mapSize.X || neighborPos.Y < 0 || neighborPos.Y >= _mapSize.Y || IsObstacle(neighborPos))
				continue;

			int index = CoordinateToIndex(neighborPos, _mapSize);

			if (map.count(index) == 0)
				continue;

			AGridNode* neighborNode = map[index];

			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString("Neighbor:") + neighborNode->ToString());
			*/

			bool isInOpenSet = find(openSet.begin(), openSet.end(), neighborNode) != openSet.end();

			int tentativeGScore = currentNode->GScore + 1;

			if (tentativeGScore >= neighborNode->GScore)
				continue;

			neighborNode->GScore = tentativeGScore;
			neighborNode->HScore = Distance(neighborPos, targetPos);
			neighborNode->Parent = currentNode;

			if (isInOpenSet)
				continue;

			openSet.push_back(neighborNode);
		}
	}

	return path;
}

bool AGameManager::IsObstacle(FIntPoint source)
{
	for (auto obstacle : obstacles) {
		if (source == obstacle)
			return true;
	}

	return false;
}

int AGameManager::CoordinateToIndex(FIntPoint coordinate, FIntPoint mapSize) {
	return coordinate.X * mapSize.Y + coordinate.Y;
}

FIntPoint AGameManager::IndexToCoordinate(int index, FIntPoint mapSize) {
	return { index / mapSize.Y,index %mapSize.Y };
}

int AGameManager::Distance(FIntPoint source, FIntPoint target) {

	// Manhattan Distance
	return abs(target.X - source.X) + abs(target.Y - source.Y);
}

vector<AGridNode*> AGameManager::ReconstructPath(AGridNode *targetNode)
{
	vector<AGridNode*> path;
	AGridNode *current = targetNode;

	while (current != nullptr) {
		path.push_back(current);

		current = current->Parent;

		if (current != nullptr && current->Parent != nullptr) {
			current->Render(GridNodeState::Through);
		}
	}

	reverse(path.begin(), path.end());

	return path;
}