
#include "ScrAppMain.h"

#include "ScrAppMainGameNode.h"
#include "../../AppMultiTime.h"
#include "../../DATA/TThread.h"
#include "../../FILEIO/InfoFile.h"

#ifndef WIN32
#include <sched.h>
#endif
#include "GameMain.h"

_THREAD_H g_fg_thread = NULL;
bool g_fg_thread_running = false;
ScrAppMain *g_fg_Parent;

void stopFindGames();
_THREAD_RETVALUE thStartFindGames(void* data);
void addGamesInFolder(TString path, ArrayList<ScrAppMainGameNode> &nodes, int depth);
void addHelpNode(ArrayList<ScrAppMainGameNode> &nodes);
void setThreadPriority(_THREAD_H *thread, bool isMax);

void startFindGames(ScrAppMain *data) {
	
	 stopFindGames();

	g_fg_thread_running = true;
	g_fg_Parent = data;

	_THREAD_ID id;
	_THREAD_RUN(thStartFindGames, (void*)NULL, id, g_fg_thread);

	setThreadPriority(&g_fg_thread, false);
}

void stopFindGames() {
#ifdef WIN32
	if (g_fg_thread != NULL) {
		g_fg_thread_running = false;
		while(g_fg_thread != NULL);
	}
	g_fg_thread = NULL;
#else
	if (g_fg_thread != NULL) {
		g_fg_thread_running = false;
		pthread_join(g_fg_thread, NULL);
	}
	g_fg_thread = NULL;
#endif
}

void setThreadPriority(_THREAD_H *thread, bool isMax) {
#ifndef WIN32
	struct sched_param sched_param;
	int policy;

	pthread_getschedparam(g_fg_thread, &policy, &sched_param);
	if (!isMax)
		sched_param.sched_priority = 0;//sched_get_priority_min(policy);
	else
		sched_param.sched_priority = sched_get_priority_max(policy);

	pthread_setschedparam(g_fg_thread, policy, &sched_param);
#endif
}


_THREAD_RETVALUE thStartFindGames(void* data) {
	do{	
		ArrayList<ScrAppMainGameNode> &nodes = g_fg_Parent->getList();

		TString full_path, inner_path, extern_path;
		full_path = GameMain::getInstance()->pathGames;
		int find_sharp = full_path.indexOf("#");
		if (find_sharp == -1) {
			inner_path = full_path;
		} else {
			inner_path = full_path.substring(0, find_sharp);
			extern_path = full_path.substring(find_sharp+1);
		}

		nodes.RemoveAll();
		addGamesInFolder(inner_path, nodes, 0);
		addGamesInFolder(extern_path, nodes, 0);

		for(int i=0; i<nodes.GetLength() && g_fg_thread_running; i++) {
			nodes[i].loadDataWithSubThread();
		}

		if (g_fg_Parent->getState() == GameListStateListLoading)
			g_fg_Parent->setState(GameListStateIdle);

		g_fg_thread = NULL;
		g_fg_thread_running = false;
	}while(false);

	_THREAD_END;

	return NULL;
}

void checkAndInsertGame(ArrayList<ScrAppMainGameNode> &nodes, const TString &path, bool isArchive) {
	int i, len = nodes.GetLength();
	for(i=0; i<len; i++)
		if (nodes[i].getPath() == path && nodes[i].isArchive() == isArchive)
			break;

	if (i == len) {
		ScrAppMainGameNode node;
		node.set(path, isArchive);
		nodes.Add(node);
	}
}

void addGamesInFolder(TString path, ArrayList<ScrAppMainGameNode> &nodes, int depth)
{
	TFolder folder;
	TString RTPNAME = _ST(GRP_RTP_USER_DATA).toUpper();

	if (path.getSize() > 0) {
		char c = path.getTextUTF8()[path.getSize()-1];
		if (c != '/' && c != '\\')
			path = path + "/";
	}

	if (folder.open(path)) {
		do{
			if (!folder.isfolder()) {
				TString name = folder.getName();

				if (GameMain::getInstance()->pathGame == path)
					g_fg_Parent->setPosition(nodes.GetLength());

				if (name.toUpper().indexOf(".ZIP") != -1 && (depth != 0 || name.toUpper() != RTPNAME)) {
					checkAndInsertGame(nodes, path + name, true);
				} else if (name.indexOf("RPG_RT.ldb") != -1) {
					checkAndInsertGame(nodes, path, false);
					folder.close();
					break;
				}
			}
		}while(folder.next());

		folder.close();
	}

	if (folder.open(path)) {
		do{
			if (folder.isfolder() && folder.get() != path)
				addGamesInFolder(folder.get() + "/", nodes, depth + 1);
		}while(folder.next());
		folder.close();
	}
}

void addHelpNode(ArrayList<ScrAppMainGameNode> &nodes) {
	ScrAppMainGameNode node;
	node.setHelp();
	nodes.Add(node);
}
