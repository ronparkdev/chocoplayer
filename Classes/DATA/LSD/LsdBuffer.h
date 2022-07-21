#ifndef __R2K_DATA__LSD_CHARACTER_BUFFER__
#define __R2K_DATA__LSD_CHARACTER_BUFFER__

using namespace std;

class GameMain;

class LsdBuffer{
	GameMain *m_main;

	bool *m_skill;
	int *m_itemcnt;
	int *m_itemused;
	int m_heroLen;
	int m_itemLen;
	int m_skillLen;

	int *m_itemcnt_bak;
	int *m_itemused_bak;
public:
	LsdBuffer(GameMain *main);
	~LsdBuffer();
	void Init();

	void Save();
	void Load();
	
	bool GetSkill(int hID, int sID);
	void SetSkill(int hid, int sID, bool val);
	
	int GetItem(int iID);
	void SetItem(int iID, int val);
	void AddItem(int iID, int val);
	void UseItem(int iID);
	void ClsUseItem(int iID);
	void ClsSkill( int heroID );
	
	void BackupItem();
	void RestoreItem();
};

#endif