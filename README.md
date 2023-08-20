## A* algorithm implementation in Unreal Engine (C++)

- Unreal 4.20
- Use Unreal's TopDown project template

#### Psuedo code

```cpp

vector<Node*> AStar(Node* start, Node* target){
    vector<Node*> openSet = {start};
    start->gScore = 0;
    //Heuristic Function can be customize, ex: Manhattan Distance,|x2-x1| + |y2-y1| 
    start->hScore = Heuristic(start,target);

    while(!openSet.empty())
    {
        //To sort openSet according to fScore (gScore + hScore) in ascending order 
        sort(openSet.begin(), openSet.end(), NodeComparison());
        
        Node* current = openSet.front();
        openSet.erase(openSet.begin());

        if(current == target)
        {
            return ReconstructPath(target);
        }

        foreach(auto neighbor in current->neighbors)
        {
            if(neighbor is obstacle or out of boundaries)
                continue;

            int tentativeGScore = current->gScore + 1;

            if(tentativeGScore < neighbor->gScore)
            {
                neighbor->gScore = tentativeGScore;
                neighbor->hScore = Heuristic(neighbor,target);
                neighbor->parent = current;
                if(neighbor not in openSet)
                    openSet.push_back(neighbor);
            }
        }

    }

    return nullptr;
}

vector<Node*> ReconstructPath(Node* target)
{
    vector<Node*> path;
	Node* current = target;

	while (current != nullptr) {
		path.push_back(current);
		current = current->Parent;
	}

	reverse(path.begin(), path.end());

	return path;
}

```

#### Demo

https://github.com/cps9257/AStar-Unreal/assets/15887567/f9836262-1209-4814-a5cc-34e8dbfff55e

#### References

https://en.wikipedia.org/wiki/A*_search_algorithm
