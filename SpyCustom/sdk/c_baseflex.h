#ifndef C_STUDIOFLEX_H
#define C_STUDIOFLEX_H
#pragma once


#include "c_baseanimating.h"
#include "c_baseanimatingoverlay.h"
#include "sceneentity_shared.h"

#include "utlvector.h"

class CFlexSceneFile
{
public:
	enum
	{
		MAX_FLEX_FILENAME = 128,
	};

	char			filename[MAX_FLEX_FILENAME];
	void* buffer;
};

struct Emphasized_Phoneme;
class CSentence;

enum
{
	PHONEME_CLASS_WEAK = 0,
	PHONEME_CLASS_NORMAL,
	PHONEME_CLASS_STRONG,

	NUM_PHONEME_CLASSES
};

struct FS_LocalToGlobal_t
{
	explicit FS_LocalToGlobal_t() :
		m_Key(0),
		m_nCount(0),
		m_Mapping(0)
	{
	}

	explicit FS_LocalToGlobal_t(const flexsettinghdr_t* key) :
		m_Key(key),
		m_nCount(0),
		m_Mapping(0)
	{
	}

	void SetCount(int count)
	{
		Assert(!m_Mapping);
		Assert(count > 0);
		m_nCount = count;
		m_Mapping = new int[m_nCount];
		Q_memset(m_Mapping, 0, m_nCount * sizeof(int));
	}

	FS_LocalToGlobal_t(const FS_LocalToGlobal_t& src)
	{
		m_Key = src.m_Key;
		delete m_Mapping;
		m_Mapping = new int[src.m_nCount];
		Q_memcpy(m_Mapping, src.m_Mapping, src.m_nCount * sizeof(int));

		m_nCount = src.m_nCount;
	}

	~FS_LocalToGlobal_t()
	{
		delete m_Mapping;
		m_nCount = 0;
		m_Mapping = 0;
	}

	const flexsettinghdr_t* m_Key;
	int						m_nCount;
	int* m_Mapping;
};

bool FlexSettingLessFunc(const FS_LocalToGlobal_t& lhs, const FS_LocalToGlobal_t& rhs);

class IHasLocalToGlobalFlexSettings
{
public:
	virtual void		EnsureTranslations(const flexsettinghdr_t* pSettinghdr) = 0;
};

struct Emphasized_Phoneme
{
	char			classname[64];
	bool			required;
	bool			basechecked;
	const flexsettinghdr_t* base;
	const flexsetting_t* exp;

	bool			valid;
	float			amount;
};

class C_BaseFlex : public C_BaseAnimatingOverlay, public IHasLocalToGlobalFlexSettings
{
	DECLARE_CLASS(C_BaseFlex, C_BaseAnimatingOverlay);
public:
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();

	C_BaseFlex();
	virtual			~C_BaseFlex();

	virtual void Spawn();

	virtual void InitPhonemeMappings();

	void		SetupMappings(char const* pchFileRoot);

	virtual CStudioHdr* OnNewModel(void);

	virtual void	StandardBlendingRules(CStudioHdr* hdr, Vector pos[], Quaternion q[], float currentTime, int boneMask);

	virtual void OnThreadedDrawSetup();

	virtual void BuildTransformations(CStudioHdr* pStudioHdr, Vector* pos, Quaternion q[], const matrix3x4_t& cameraTransform, int boneMask, CBoneBitList& boneComputed);
	static void		LinkToGlobalFlexControllers(CStudioHdr* hdr);
	virtual	void	SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights);
	virtual	bool	SetupGlobalWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights);
	static void		RunFlexDelay(int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights, float& flFlexDelayTime);
	virtual	void	SetupLocalWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights);
	virtual bool	UsesFlexDelayedWeights();

	static	void	RunFlexRules(CStudioHdr* pStudioHdr, float* dest);

	virtual Vector	SetViewTarget(CStudioHdr* pStudioHdr);

	virtual bool	GetSoundSpatialization(SpatializationInfo_t& info);

	virtual void	GetToolRecordingState(KeyValues* msg);

	void				AddFlexAnimation(CSceneEventInfo* info);

	void			SetFlexWeight(LocalFlexController_t index, float value);
	float			GetFlexWeight(LocalFlexController_t index);

	LocalFlexController_t				FindFlexController(const char* szName);

public:
	Vector			m_viewtarget;
	CInterpolatedVar< Vector >	m_iv_viewtarget;
	float			m_flexWeight[MAXSTUDIOFLEXCTRL];
	CInterpolatedVarArray< float, MAXSTUDIOFLEXCTRL >	m_iv_flexWeight;

	int				m_blinktoggle;

	static int		AddGlobalFlexController(const char* szName);
	static char const* GetGlobalFlexControllerName(int idx);

public:

	void				StartChoreoScene(CChoreoScene* scene);
	void				RemoveChoreoScene(CChoreoScene* scene);

	virtual bool		StartSceneEvent(CSceneEventInfo* info, CChoreoScene* scene, CChoreoEvent* event, CChoreoActor* actor, C_BaseEntity* pTarget);

	virtual void		ProcessSceneEvents(bool bFlexEvents);

	virtual	bool		ProcessSceneEvent(bool bFlexEvents, CSceneEventInfo* info, CChoreoScene* scene, CChoreoEvent* event);

	virtual bool		ProcessSequenceSceneEvent(CSceneEventInfo* info, CChoreoScene* scene, CChoreoEvent* event);

	void				ClearSceneEvents(CChoreoScene* scene, bool canceled);

	virtual	bool		ClearSceneEvent(CSceneEventInfo* info, bool fastKill, bool canceled);

	void				AddSceneEvent(CChoreoScene* scene, CChoreoEvent* event, C_BaseEntity* pTarget = NULL, bool bClientSide = false);

	void				RemoveSceneEvent(CChoreoScene* scene, CChoreoEvent* event, bool fastKill);

	bool				CheckSceneEvent(float currenttime, CChoreoScene* scene, CChoreoEvent* event);

	virtual bool		CheckSceneEventCompletion(CSceneEventInfo* info, float currenttime, CChoreoScene* scene, CChoreoEvent* event);

	int					FlexControllerLocalToGlobal(const flexsettinghdr_t* pSettinghdr, int key);

	virtual void		EnsureTranslations(const flexsettinghdr_t* pSettinghdr);

	void* FindSceneFile(const char* filename);

private:

	bool RequestStartSequenceSceneEvent(CSceneEventInfo* info, CChoreoScene* scene, CChoreoEvent* event, CChoreoActor* actor, CBaseEntity* pTarget);

	bool ProcessFlexAnimationSceneEvent(CSceneEventInfo* info, CChoreoScene* scene, CChoreoEvent* event);
	bool ProcessFlexSettingSceneEvent(CSceneEventInfo* info, CChoreoScene* scene, CChoreoEvent* event);
	void AddFlexSetting(const char* expr, float scale,
		const flexsettinghdr_t* pSettinghdr, bool newexpression);

	CUtlVector < CSceneEventInfo >		m_SceneEvents;
	CUtlVector < CChoreoScene* >		m_ActiveChoreoScenes;

	bool				HasSceneEvents() const;

private:
	CUtlRBTree< FS_LocalToGlobal_t, unsigned short > m_LocalToGlobal;

	float			m_blinktime;
	int				m_prevblinktoggle;

	int				m_iBlink;
	LocalFlexController_t				m_iEyeUpdown;
	LocalFlexController_t				m_iEyeRightleft;
	bool			m_bSearchedForEyeFlexes;
	int				m_iMouthAttachment;

	float			m_flFlexDelayTime;
	float* m_flFlexDelayedWeight;
	int				m_cFlexDelayedWeight;

	static int		g_numflexcontrollers;
	static char* g_flexcontroller[MAXSTUDIOFLEXCTRL * 4];       
	static float	g_flexweight[MAXSTUDIOFLEXDESC];

protected:

	Emphasized_Phoneme m_PhonemeClasses[NUM_PHONEME_CLASSES];

private:

	C_BaseFlex(const C_BaseFlex&);     

	const flexsetting_t* FindNamedSetting(const flexsettinghdr_t* pSettinghdr, const char* expr);

	void			ProcessVisemes(Emphasized_Phoneme* classes);
	void			AddVisemesForSentence(Emphasized_Phoneme* classes, float emphasis_intensity, CSentence* sentence, float t, float dt, bool juststarted);
	void			AddViseme(Emphasized_Phoneme* classes, float emphasis_intensity, int phoneme, float scale, bool newexpression);
	bool			SetupEmphasisBlend(Emphasized_Phoneme* classes, int phoneme);
	void			ComputeBlendedSetting(Emphasized_Phoneme* classes, float emphasis_intensity);

#ifdef HL2_CLIENT_DLL
public:

	Vector			m_vecLean;
	CInterpolatedVar< Vector >	m_iv_vecLean;
	Vector			m_vecShift;
	CInterpolatedVar< Vector >	m_iv_vecShift;
#endif
};


class CFlexSceneFileManager : CAutoGameSystem
{
public:

	CFlexSceneFileManager() : CAutoGameSystem("CFlexSceneFileManager")
	{
	}

	virtual bool Init();
	virtual void Shutdown();

	void EnsureTranslations(IHasLocalToGlobalFlexSettings* instance, const flexsettinghdr_t* pSettinghdr);
	void* FindSceneFile(IHasLocalToGlobalFlexSettings* instance, const char* filename, bool allowBlockingIO);

private:
	void DeleteSceneFiles();

	CUtlVector< CFlexSceneFile* > m_FileList;
};


inline bool C_BaseFlex::HasSceneEvents() const
{
	return m_SceneEvents.Count() != 0;
}


EXTERN_RECV_TABLE(DT_BaseFlex);

float* GetVisemeWeights(int phoneme);


#endif  
