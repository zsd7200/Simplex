#include "MyOctant.h"
using namespace Simplex;

// initialize static variables
uint MyOctant::m_uOctantCount = 0;
uint MyOctant::m_uMaxLevel = 0;
uint MyOctant::m_uIdealEntityCount = 0;

//  public methods
MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init();

	// set up variables
	m_uOctantCount = 0;
	m_uID = m_uOctantCount;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_pRoot = this;

	// get entity list for size of loop and to get a rigidbody for max/min list
	std::vector<MyEntity*> l_Entity_List = m_pEntityMngr->GetEntityList();
	uint iEntityCount = l_Entity_List.size();

	std::vector<vector3> v3MaxMin_list;

	// get max/min values to create temporary rigidbody
	for (uint i = 0; i < iEntityCount; ++i)
	{
		MyRigidBody* pRG = l_Entity_List[i]->GetRigidBody();
		vector3 v3Min = pRG->GetMinGlobal();
		vector3 v3Max = pRG->GetMaxGlobal();
		v3MaxMin_list.push_back(v3Min);
		v3MaxMin_list.push_back(v3Max);
	}

	MyRigidBody* m_pRigidBody = new MyRigidBody(v3MaxMin_list);

	vector3 v3HalfWidth = m_pRigidBody->GetHalfWidth();
	float max = v3HalfWidth.x; //set max to x

	// check to see if any other half-width values are higher than max
	for (size_t i = 0; i < 3; i++)
		if (max < v3HalfWidth[i])
			max = v3HalfWidth[i];

	m_fSize = max * 2;

	// set center, min, and max
	m_v3Center = m_pRigidBody->GetCenterLocal();
	m_v3Min = m_v3Center - vector3(max);
	m_v3Max = m_v3Center + vector3(max);

	// delete rigidbody to prevent memory leaks
	SafeDelete(m_pRigidBody);

	// increment count
	m_uOctantCount++;

	// only construct the tree if maxLevel is above 0
	if (a_nMaxLevel != 0)
	{
		ConstructTree(m_uMaxLevel);
	}
}
MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	Init();

	// set up variables
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	// increment count
	m_uOctantCount++;
}
MyOctant::MyOctant(MyOctant const& other)
{
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_uChildren = other.m_uChildren;
	m_fSize = other.m_fSize;
	m_pMeshMngr = other.m_pMeshMngr;
	m_pEntityMngr = other.m_pEntityMngr;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
	m_pParent = other.m_pParent;
	m_EntityList = other.m_EntityList;
	m_pRoot = other.m_pRoot;
	m_lChild = other.m_lChild;

	for (size_t i = 0; i < 8; i++)
		m_pChild[i] = other.m_pChild[i];
}
MyOctant& MyOctant::operator=(MyOctant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}
MyOctant::~MyOctant() { Release(); }
void MyOctant::Swap(MyOctant& other)
{
	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_uChildren, other.m_uChildren);
	std::swap(m_fSize, other.m_fSize);
	std::swap(m_pMeshMngr, other.m_pMeshMngr);
	std::swap(m_pEntityMngr, other.m_pEntityMngr);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
	std::swap(m_pParent, other.m_pParent);
	std::swap(m_pChild, other.m_pChild);
	std::swap(m_EntityList, other.m_EntityList);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);
}

// accessors
float MyOctant::GetSize(void) { return m_fSize; }
vector3 MyOctant::GetCenterGlobal(void) { return m_v3Center; }
vector3 MyOctant::GetMinGlobal(void) { return m_v3Min; }
vector3 MyOctant::GetMaxGlobal(void) { return m_v3Max; }

bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	// get rigidbody of entity with RBIndex
	MyEntity* entity = m_pEntityMngr->GetEntity(a_uRBIndex);
	MyRigidBody* rigidBody = entity->GetRigidBody();

	// get min/max variables
	vector3 minGlobal = rigidBody->GetMinGlobal();
	vector3 maxGlobal = rigidBody->GetMaxGlobal();

	// check x
	if (m_v3Min.x > maxGlobal.x)
		return false;
	if (m_v3Max.x < minGlobal.x)
		return false;

	// check y
	if (m_v3Min.y > maxGlobal.y)
		return false;
	if (m_v3Max.y < minGlobal.y)
		return false;

	// check z
	if (m_v3Min.z > maxGlobal.z)
		return false;
	if (m_v3Max.z < minGlobal.z)
		return false;

	return true;
}
void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// display wire cube if uID matches a_nIndex
	if (m_uID == a_nIndex)
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color);
	
	// otherwise, loop recursively to display all wire cubes necessary
	else
		for (uint i = 0; i < m_uChildren; i++)
			m_pChild[i]->Display(a_nIndex);
}
void MyOctant::Display(vector3 a_v3Color)
{
	// essentially the same as the other display method, just recursively calling itself until it no longer has children
	for (int i = 0; i < m_uChildren; i++)
		m_pChild[i]->Display(a_v3Color);

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color);
}
void MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	// same as display
	for (size_t i = 0; i < m_lChild.size(); i++)
		m_lChild[i]->DisplayLeafs(a_v3Color);

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(vector3(m_fSize)), a_v3Color);
}
void MyOctant::ClearEntityList(void){ m_EntityList.clear(); }
void MyOctant::Subdivide()
{
	// check if the level is higher than max or if it is already subdivided
	if (m_uLevel >= m_uMaxLevel || m_uChildren != 0)
		return;

	m_uChildren = 8;

	// variables to adjust pChild location/size
	float fHalfSize = m_fSize / 2;
	float fQuartSize = m_fSize / 4;

	// create new octants for each child in array
	m_pChild[0] = new MyOctant(m_v3Center + vector3(fQuartSize, fQuartSize, fQuartSize), fHalfSize);
	m_pChild[1] = new MyOctant(m_v3Center + vector3(-fQuartSize, fQuartSize, fQuartSize), fHalfSize);
	m_pChild[2] = new MyOctant(m_v3Center + vector3(-fQuartSize, -fQuartSize, fQuartSize), fHalfSize);
	m_pChild[3] = new MyOctant(m_v3Center + vector3(fQuartSize, -fQuartSize, fQuartSize), fHalfSize);

	m_pChild[4] = new MyOctant(m_v3Center + vector3(fQuartSize, fQuartSize, -fQuartSize), fHalfSize);
	m_pChild[5] = new MyOctant(m_v3Center + vector3(-fQuartSize, fQuartSize, -fQuartSize), fHalfSize);
	m_pChild[6] = new MyOctant(m_v3Center + vector3(-fQuartSize, -fQuartSize, -fQuartSize), fHalfSize);
	m_pChild[7] = new MyOctant(m_v3Center + vector3(fQuartSize, -fQuartSize, -fQuartSize), fHalfSize);

	// set up pChildren
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_pRoot = m_pRoot;

		// check if children need to be subdivided
		if (m_pChild[i]->ContainsMoreThan(m_uIdealEntityCount) == true)
			m_pChild[i]->Subdivide();
	}
}
MyOctant* MyOctant::GetChild(uint a_nChild) { return m_pChild[a_nChild]; }
MyOctant* MyOctant::GetParent(void) { return m_pParent; }
bool MyOctant::IsLeaf(void)
{
	// if no children, it is a leaf
	if (m_uChildren == 0)
		return true;
	else
		return false;
}
bool MyOctant::ContainsMoreThan(uint a_nEntities)
{
	// count, incremented if there is a collision
	uint count = 0;

	for (size_t i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		if (IsColliding(i) == true)
			count++;
		if (count > a_nEntities)
			return true;
	}

	return false;
}
void MyOctant::KillBranches(void)
{
	// loop through for number of children and delete m_pChild until there are none left
	for (size_t i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->KillBranches();
		SafeDelete(m_pChild[i]);
	}

	// reset children back to 0
	m_uChildren = 0;
}
void MyOctant::ConstructTree(uint a_nMaxLevel)
{
	// set up variables
	m_uMaxLevel = a_nMaxLevel;
	m_uOctantCount = 1;

	// subdivide if containsmore returns true
	if (ContainsMoreThan(m_uIdealEntityCount) == true)
		Subdivide();

	// assign IDs and populate m_lChild
	AssignIDtoEntity();
	ConstructList();
}
void MyOctant::AssignIDtoEntity(void)
{
	// if there are children, loop for amount of children and recursively assign IDs
	if (m_uChildren != 0)
		for (size_t i = 0; i < m_uChildren; i++)
			m_pChild[i]->AssignIDtoEntity();

	else
		for (size_t i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
			if (IsColliding(i) == true)
			{
				m_EntityList.push_back(i);
				m_pEntityMngr->AddDimension(i, m_uID);
			}

}
uint MyOctant::GetOctantCount(void) { return m_uOctantCount; }

// private methods
void MyOctant::Release(void)
{
	// kill all branches if root
	if (m_uLevel == 0)
		KillBranches();

	// reset other variables
	m_uChildren = 0;
	m_fSize = 0;
	m_EntityList.clear();
	m_lChild.clear();
}
void MyOctant::Init(void)
{
	// initialize all base variables

	m_v3Center = vector3(0);
	m_v3Min = vector3(0);
	m_v3Max = vector3(0);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	MyOctant* m_pParent = nullptr;
	for (uint i = 0; i < 8; i++)
		m_pChild[i] = nullptr;

	m_pRoot = nullptr;

	m_uID = m_uOctantCount;
	m_uLevel = 0;
	m_uChildren = 0;
	m_fSize = 0;
}
void MyOctant::ConstructList(void)
{
	// make sure there are elements in pChild array
	if (m_pChild[0] != nullptr)
		for (size_t i = 0; i < m_uChildren; i++)
			// recursively call constructlist to fill m_lChild
			m_pChild[i]->ConstructList();

	if (m_EntityList.size() > 0)
		m_pParent->m_lChild.push_back(this);
}