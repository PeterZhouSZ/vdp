#pragma once
#include "MeshDefinition.h"
class HierarchicalClustering;
class MinimalSpanningTree;
class MeshCut;

// Class of cut a closed mesh
class CutClosedMesh
{
public:
	CutClosedMesh(const Mesh & mesh);
	virtual ~CutClosedMesh(void);

	// Initialization
	virtual void Init(const int & s = 13,
		const bool & issaveintermediate = false,
		const bool & isgeodesic = false,
		const int & nvsimplified = 13000,
		const int & nrandomcut = 10,
		const int & nvotes = 2,
		const int & npostprocess = 5,
		const std::string & filename = std::string());

	// Run the algorithm
	virtual void Run(void) = 0;

protected:
	// Remeshing
	bool Remesh(const int & targetnv);

	// Hierarchical clustering of landmarks
	bool ClusterLandmarks(const int & size) const;

	// Initialization
	bool InitialCutMesh(const Mesh & mesh);

	// Find a spanning tree and cut the mesh
	bool CutMesh(const std::vector<int> & landmarks);

	// Run parameterization (Free boundary, KP-Newton)
	bool Parameterization(void);

	// Get original vertex indices
	std::vector<int> FindOrginalVertexIndex(const std::vector<int> & landmarks) const;

	// Voting landmarks
	std::vector<std::pair<int, int>> VoteLandmarks(const std::vector<std::vector<int>>& intermediatelandmarks) const;

	// Remove nearby lamdmarks on the remeshed mesh
	std::vector<int> RemoveNearbyLandmarks(const std::vector<std::pair<int, int>> & landmarkwwithvote) const;

	// Save cut information to a file
	static bool SaveCuts(const std::string & filename, const std::vector<int> & iscutvertices, const std::vector<int> & iscutedges);

	// Save landmark information to a file
	static bool SaveLandmarks(const std::string & filename, const std::vector<int> & landmarks);

	const Mesh & orimesh; // constant reference to the original mesh
	Mesh currentmesh;     // current mesh
	Mesh remesh;          // remeshed mesh
	std::vector<int> originalvid; // map from remesh to the original mesh
	std::unique_ptr<HierarchicalClustering> hc; // hierarchical clustering
	std::unique_ptr<MinimalSpanningTree> mst;   // minimal spanning tree
	std::unique_ptr<MeshCut> mc;                // mesh cutter
	int size = 13;                 // local feature size N
	bool saveintermediate = false; // whether output intermediate results
	bool geodesic = false;         // use geodesic or not
	int nvsimplify = 13000;        // vertex number of simplified mesh
	int nrandom = 10;              // number of random cutting process
	int nvoting = 2;               // voting threshold
	int npost = 5;                 // n-ring neighborhood filtering
	std::string basefile;  // base file name
};
