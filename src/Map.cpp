#include "Map.h"

#include "Enemy.h"
#include "Model.h"
#include "GameObject.h"
#include "Player.h"
#include "Renderer.h"

#ifdef RASPBERRY
const std::string rootFilepath = "../../../";
#elif WIN32
const std::string rootFilepath("../");
#endif

Map::Map(Renderer* rend, Camera* cam, Shader* shader, btDiscreteDynamicsWorld* physicsWorld, EnemyManager* manager, ObjectManager* p_objManager)
	:p_cam(cam), p_shader(shader), physicsWorld(physicsWorld), p_renderer(rend), p_managerEnemies(manager), p_objManager(p_objManager) {}

Map::~Map()
{
}

void Map::CSVparser(FILE* file, int p_map[100][100])
{
	float wholeNum = 0;
    int x = 0, y = 0;

    while (1) {
	    const char c = static_cast<char>(fgetc(file));
        if (feof(file)) {
            break;
        }

        if (c == ',')
        {
            wholeNum /= 10;
            if (flipValue) { wholeNum *= -1; }

            p_map[x][y] = static_cast<int>(wholeNum);
            wholeNum = 0;
            flipValue = false;
            x++;

        }
        else if (c == '-') {
            flipValue = true;
        }
        else if (c == '\n')
        {
            wholeNum = 0;
            y++;
            x = 0;
        }
        else
        {
            wholeNum = wholeNum + ((int)c - 48);
            wholeNum *= 10;
        }
    }

    fclose(file);
}

void Map::ReBuildLevel(std::string CSVpath, float offset, std::string wallFilepath, std::string groundFilepath)
{
    for (int i = 0; i < level.size(); i++)
    {
        level[i]->removePhysicsComponent(physicsWorld);
	    delete level[i];
    	level[i] = nullptr;
    }
    level.clear();
	p_managerEnemies->deleteEnemies();

    delete m_ground;
    delete m_walls;
    m_ground = nullptr;
    m_walls = nullptr;

    BuildLevel(CSVpath, offset, wallFilepath, groundFilepath);
}

void Map::BuildLevel(std::string CSVpath, float offset, std::string groundFilepath, std::string wallFilepath)
{
    m_ground = new Model(groundFilepath );
    m_walls =  new Model(wallFilepath );

	glm::ivec2 mapSize = { 100, 100 }; //could I read this from the CSV file? 
    FILE* drawMap = fopen(CSVpath.c_str(), "r");
    CSVparser(drawMap, map);

    int counter = 0;
    for (int x = 0; x < mapSize.x; x++) {
        for (int z = 0; z < mapSize.y; z++)
        {
            switch (map[x][z])
            {
            case 0: //FLOORS
                level.push_back(new GameObject(m_ground, p_cam, p_shader, glm::vec3((-mapSize.x / 2.f + x) * offset, 0.f, (-mapSize.y / 2.f + z) * offset), glm::vec3(0.f), glm::vec3(2.f, 2.f, 2.f)));
                level[counter]->addPhysicsComponent(0.f, glm::vec3(0.f), glm::vec3(2.f) * 2.f, physicsWorld,
                    GROUND, ALL);
                level[counter]->Update();
                counter++;
            	break;

            case 1: //WALLS
                level.push_back(new GameObject(m_walls, p_cam, p_shader, glm::vec3((-mapSize.x / 2 + x) * offset, 2, (-mapSize.y / 2 + z) * offset), glm::vec3(0), glm::vec3(2.f, 2.f, 2.f)));
                level.push_back(new GameObject(m_walls, p_cam, p_shader, glm::vec3((-mapSize.x / 2 + x) * offset, 6, (-mapSize.y / 2 + z) * offset), glm::vec3(0), glm::vec3(2.f, 2.f, 2.f)));
                level[counter]->addPhysicsComponent(0.f, glm::vec3(0.f), glm::vec3(2.f, 8.f, 2.f) * 2.f, physicsWorld, GROUND, ALL);
                level[counter + 1]->Update();
                counter += 2;
                break;

            case 2: //LEVEL WIN Position

                break;


            case 3: //SPAWN POINT + FLOOR
                level.push_back(new GameObject(m_ground, p_cam, p_shader, glm::vec3((-mapSize.x / 2 + x) * offset, 0, (-mapSize.y / 2 + z) * offset), glm::vec3(0), glm::vec3(2.f, 2.f, 2.f)));
                level[counter]->addPhysicsComponent(0.f, glm::vec3(0.f), glm::vec3(2.f) * 2.f, physicsWorld,
                    GROUND, ALL);
                level[counter]->Update();
                p_player->getObject()->getTransComponent()->WorldPosition = glm::vec3((-mapSize.x / 2 + x) * offset, 4.f, (-mapSize.y / 2 + z) * offset);
                p_player->getObject()->setColliderPos(p_player->getObject()->getPhysicsComponent()->m_offset);
                counter++;

                break;

            case 4: //ENEMY + FLOOR
                level.push_back(new GameObject(m_ground, p_cam, p_shader, glm::vec3((-mapSize.x / 2 + x) * offset, 0, (-mapSize.y / 2 + z) * offset), glm::vec3(0), glm::vec3(2.f, 2.f, 2.f)));
                level[counter]->addPhysicsComponent(0.f, glm::vec3(0.f), glm::vec3(2.f) * 2.f, physicsWorld,
                    GROUND, ALL);
                level[counter]->Update();

                p_managerEnemies->createEnemy(BASIC, glm::vec3((-mapSize.x / 2 + x) * offset, 4.f, (-mapSize.y / 2 + z) * offset));
            	counter++;

                break;
            }
        }
    }
}


void Map::Draw()
{
	for (auto tile : level) {
		const float dist = distance(tile->getTransComponent()->WorldPosition, p_player->getObject()->getTransComponent()->WorldPosition);
        if (dist < distanceCull) {
            tile->PassUniforms();
            p_renderer->Draw(tile, tile->getShader());
        }
	}
}
