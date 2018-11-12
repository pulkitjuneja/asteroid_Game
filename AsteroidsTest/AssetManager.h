#ifndef ASSETMANAGER_H_INCLUDED
#define ASSETMANAGER_H_INCLUDED

#include <string>
#include <map>

using namespace std;

template <class ASSET>
class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	ASSET *Create(const std::string &assetId,
		const typename ASSET::CreateParams *params);
	ASSET *Get(const std::string &assetId) const;
	void Destroy(const std::string &assetId);

private:
	typedef map<string, ASSET*> AssetMap;
	AssetManager(const AssetManager &);
	void operator=(const AssetManager &);
	AssetMap assets_;
};

#include "AssetManager.hpp"

#endif // ASSETMANAGER_H_INCLUDED
