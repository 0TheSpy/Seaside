#ifndef VPHYSICS_INTERFACE_H
#define VPHYSICS_INTERFACE_H
#ifdef _WIN32
#pragma once
#endif


#include "interface.h"
#include "IAppSystem.h"
#include "vector.h"
#include "vector4d.h"
#include "vcollide.h"


#define METERS_PER_INCH					(0.0254f)
#define CUBIC_METERS_PER_CUBIC_INCH		(METERS_PER_INCH*METERS_PER_INCH*METERS_PER_INCH)
#define POUNDS_PER_KG	(2.2f)
#define KG_PER_POUND	(1.0f/POUNDS_PER_KG)

#define lbs2kg(x)		((x)*KG_PER_POUND)
#define kg2lbs(x)		((x)*POUNDS_PER_KG)

const float VPHYSICS_MIN_MASS = 0.1f;
const float VPHYSICS_MAX_MASS = 5e4f;

class IPhysicsObject;
class IPhysicsEnvironment;
class IPhysicsSurfaceProps;
class IPhysicsConstraint;
class IPhysicsConstraintGroup;
class IPhysicsFluidController;
class IPhysicsSpring;
class IPhysicsVehicleController;
class IConvexInfo;
class IPhysicsObjectPairHash;
class IPhysicsCollisionSet;
class IPhysicsPlayerController;
class IPhysicsFrictionSnapshot;

struct Ray_t;
struct constraint_ragdollparams_t;
struct constraint_hingeparams_t;
struct constraint_fixedparams_t;
struct constraint_ballsocketparams_t;
struct constraint_slidingparams_t;
struct constraint_pulleyparams_t;
struct constraint_lengthparams_t;
struct constraint_groupparams_t;

struct vehicleparams_t;
struct matrix3x4_t;

struct fluidparams_t;
struct springparams_t;
struct objectparams_t;
struct debugcollide_t;
class CGameTrace;
typedef CGameTrace trace_t;
struct physics_stats_t;
struct physics_performanceparams_t;
struct virtualmeshparams_t;

struct physsaveparams_t;
struct physrestoreparams_t;
struct physprerestoreparams_t;

enum PhysInterfaceId_t
{
	PIID_UNKNOWN,
	PIID_IPHYSICSOBJECT,
	PIID_IPHYSICSFLUIDCONTROLLER,
	PIID_IPHYSICSSPRING,
	PIID_IPHYSICSCONSTRAINTGROUP,
	PIID_IPHYSICSCONSTRAINT,
	PIID_IPHYSICSSHADOWCONTROLLER,
	PIID_IPHYSICSPLAYERCONTROLLER,
	PIID_IPHYSICSMOTIONCONTROLLER,
	PIID_IPHYSICSVEHICLECONTROLLER,
	PIID_IPHYSICSGAMETRACE,

	PIID_NUM_TYPES
};


class ISave;
class IRestore;


#define VPHYSICS_DEBUG_OVERLAY_INTERFACE_VERSION	"VPhysicsDebugOverlay001"

abstract_class IVPhysicsDebugOverlay
{
public:
	virtual void AddEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, PRINTF_FORMAT_STRING const char* format, ...) = 0;
	virtual void AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& max, QAngle const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void AddTriangleOverlay(const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b,bool noDepthTest, float duration) = 0;
	virtual void AddTextOverlay(const Vector& origin, float duration, PRINTF_FORMAT_STRING const char* format, ...) = 0;
	virtual void AddTextOverlay(const Vector& origin, int line_offset, float duration, PRINTF_FORMAT_STRING const char* format, ...) = 0;
	virtual void AddScreenTextOverlay(float flXPos, float flYPos,float flDuration, int r, int g, int b, int a, const char* text) = 0;
	virtual void AddSweptBoxOverlay(const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const QAngle& angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void AddTextOverlayRGB(const Vector& origin, int line_offset, float duration, float r, float g, float b, float alpha, PRINTF_FORMAT_STRING const char* format, ...) = 0;
};

#define VPHYSICS_INTERFACE_VERSION	"VPhysics031"

abstract_class IPhysics : public IAppSystem
{
public:
	virtual	IPhysicsEnvironment * CreateEnvironment(void) = 0;
	virtual void DestroyEnvironment(IPhysicsEnvironment*) = 0;
	virtual IPhysicsEnvironment* GetActiveEnvironmentByIndex(int index) = 0;

	virtual IPhysicsObjectPairHash* CreateObjectPairHash() = 0;
	virtual void						DestroyObjectPairHash(IPhysicsObjectPairHash* pHash) = 0;

	virtual IPhysicsCollisionSet* FindOrCreateCollisionSet(unsigned int id, int maxElementCount) = 0;
	virtual IPhysicsCollisionSet* FindCollisionSet(unsigned int id) = 0;
	virtual void						DestroyAllCollisionSets() = 0;
};


class CPhysConvex;
class CPhysPolysoup;
class ICollisionQuery;
class IVPhysicsKeyParser;
struct convertconvexparams_t;
class CPackedPhysicsDescription;

class CPolyhedron;

struct truncatedcone_t
{
	Vector	origin;
	Vector	normal;
	float	h;			      
	float	theta;		   
};


#define VPHYSICS_COLLISION_INTERFACE_VERSION	"VPhysicsCollision007"

abstract_class IPhysicsCollision
{
public:
	virtual ~IPhysicsCollision(void) {}

	virtual CPhysConvex * ConvexFromVerts(Vector * *pVerts, int vertCount) = 0;
	virtual CPhysConvex* ConvexFromPlanes(float* pPlanes, int planeCount, float mergeDistance) = 0;
	virtual float			ConvexVolume(CPhysConvex* pConvex) = 0;

	virtual float			ConvexSurfaceArea(CPhysConvex* pConvex) = 0;
	virtual void			SetConvexGameData(CPhysConvex* pConvex, unsigned int gameData) = 0;
	virtual void			ConvexFree(CPhysConvex* pConvex) = 0;
	virtual CPhysConvex* BBoxToConvex(const Vector& mins, const Vector& maxs) = 0;
	virtual CPhysConvex* ConvexFromConvexPolyhedron(const CPolyhedron& ConvexPolyhedron) = 0;
	virtual void			ConvexesFromConvexPolygon(const Vector& vPolyNormal, const Vector* pPoints, int iPointCount, CPhysConvex** pOutput) = 0;

	virtual CPhysPolysoup* PolysoupCreate(void) = 0;
	virtual void			PolysoupDestroy(CPhysPolysoup* pSoup) = 0;
	virtual void			PolysoupAddTriangle(CPhysPolysoup* pSoup, const Vector& a, const Vector& b, const Vector& c, int materialIndex7bits) = 0;
	virtual CPhysCollide* ConvertPolysoupToCollide(CPhysPolysoup* pSoup, bool useMOPP) = 0;

	virtual CPhysCollide* ConvertConvexToCollide(CPhysConvex** pConvex, int convexCount) = 0;
	virtual CPhysCollide* ConvertConvexToCollideParams(CPhysConvex** pConvex, int convexCount, const convertconvexparams_t& convertParams) = 0;
	virtual void			DestroyCollide(CPhysCollide* pCollide) = 0;

	virtual int				CollideSize(CPhysCollide* pCollide) = 0;
	virtual int				CollideWrite(char* pDest, CPhysCollide* pCollide, bool bSwap = false) = 0;
	virtual CPhysCollide* UnserializeCollide(char* pBuffer, int size, int index) = 0;

	virtual float			CollideVolume(CPhysCollide* pCollide) = 0;
	virtual float			CollideSurfaceArea(CPhysCollide* pCollide) = 0;

	virtual Vector			CollideGetExtent(const CPhysCollide* pCollide, const Vector& collideOrigin, const QAngle& collideAngles, const Vector& direction) = 0;

	virtual void			CollideGetAABB(Vector* pMins, Vector* pMaxs, const CPhysCollide* pCollide, const Vector& collideOrigin, const QAngle& collideAngles) = 0;

	virtual void			CollideGetMassCenter(CPhysCollide* pCollide, Vector* pOutMassCenter) = 0;
	virtual void			CollideSetMassCenter(CPhysCollide* pCollide, const Vector& massCenter) = 0;
	virtual Vector			CollideGetOrthographicAreas(const CPhysCollide* pCollide) = 0;
	virtual void			CollideSetOrthographicAreas(CPhysCollide* pCollide, const Vector& areas) = 0;

	virtual int				CollideIndex(const CPhysCollide* pCollide) = 0;

	virtual CPhysCollide* BBoxToCollide(const Vector& mins, const Vector& maxs) = 0;
	virtual int				GetConvexesUsedInCollideable(const CPhysCollide* pCollideable, CPhysConvex** pOutputArray, int iOutputArrayLimit) = 0;


	virtual void TraceBox(const Vector& start, const Vector& end, const Vector& mins, const Vector& maxs, const CPhysCollide* pCollide, const Vector& collideOrigin, const QAngle& collideAngles, trace_t* ptr) = 0;
	virtual void TraceBox(const Ray_t& ray, const CPhysCollide* pCollide, const Vector& collideOrigin, const QAngle& collideAngles, trace_t* ptr) = 0;
	virtual void TraceBox(const Ray_t& ray, unsigned int contentsMask, IConvexInfo* pConvexInfo, const CPhysCollide* pCollide, const Vector& collideOrigin, const QAngle& collideAngles, trace_t* ptr) = 0;

	virtual void TraceCollide(const Vector& start, const Vector& end, const CPhysCollide* pSweepCollide, const QAngle& sweepAngles, const CPhysCollide* pCollide, const Vector& collideOrigin, const QAngle& collideAngles, trace_t* ptr) = 0;

	virtual bool			IsBoxIntersectingCone(const Vector& boxAbsMins, const Vector& boxAbsMaxs, const truncatedcone_t& cone) = 0;

	virtual void			VCollideLoad(vcollide_t* pOutput, int solidCount, const char* pBuffer, int size, bool swap = false) = 0;
	virtual void			VCollideUnload(vcollide_t* pVCollide) = 0;

	virtual IVPhysicsKeyParser* VPhysicsKeyParserCreate(const char* pKeyData) = 0;
	virtual void			VPhysicsKeyParserDestroy(IVPhysicsKeyParser* pParser) = 0;

	virtual int				CreateDebugMesh(CPhysCollide const* pCollisionModel, Vector** outVerts) = 0;
	virtual void			DestroyDebugMesh(int vertCount, Vector* outVerts) = 0;

	virtual ICollisionQuery* CreateQueryModel(CPhysCollide* pCollide) = 0;
	virtual void			DestroyQueryModel(ICollisionQuery* pQuery) = 0;

	virtual IPhysicsCollision* ThreadContextCreate(void) = 0;
	virtual void			ThreadContextDestroy(IPhysicsCollision* pThreadContex) = 0;

	virtual CPhysCollide* CreateVirtualMesh(const virtualmeshparams_t& params) = 0;
	virtual bool			SupportsVirtualMesh() = 0;


	virtual bool			GetBBoxCacheSize(int* pCachedSize, int* pCachedCount) = 0;


	virtual CPolyhedron* PolyhedronFromConvex(CPhysConvex* const pConvex, bool bUseTempPolyhedron) = 0;

	virtual void			OutputDebugInfo(const CPhysCollide* pCollide) = 0;
	virtual unsigned int	ReadStat(int statID) = 0;
};

abstract_class ICollisionQuery
{
public:
	virtual ~ICollisionQuery() {}
	virtual int		ConvexCount(void) = 0;
	virtual int		TriangleCount(int convexIndex) = 0;
	virtual unsigned int GetGameData(int convexIndex) = 0;
	virtual void	GetTriangleVerts(int convexIndex, int triangleIndex, Vector* verts) = 0;

	virtual void	SetTriangleVerts(int convexIndex, int triangleIndex, const Vector* verts) = 0;

	virtual int		GetTriangleMaterialIndex(int convexIndex, int triangleIndex) = 0;
	virtual void	SetTriangleMaterialIndex(int convexIndex, int triangleIndex, int index7bits) = 0;
};

abstract_class IPhysicsGameTrace
{
public:
	virtual void VehicleTraceRay(const Ray_t & ray, void* pVehicle, trace_t * pTrace) = 0;
	virtual	void VehicleTraceRayWithWater(const Ray_t& ray, void* pVehicle, trace_t* pTrace) = 0;
	virtual bool VehiclePointInWater(const Vector& vecPoint) = 0;
};

abstract_class IConvexInfo
{
public:
	virtual unsigned int GetContents(int convexGameData) = 0;
};

class CPhysicsEventHandler;
abstract_class IPhysicsCollisionData
{
public:
	virtual void GetSurfaceNormal(Vector & out) = 0;		        
	virtual void GetContactPoint(Vector& out) = 0;		       
	virtual void GetContactSpeed(Vector& out) = 0;		           
};


struct vcollisionevent_t
{
	IPhysicsObject* pObjects[2];
	int				surfaceProps[2];
	bool			isCollision;
	bool			isShadowCollision;
	float			deltaCollisionTime;

	float			collisionSpeed;				    
	IPhysicsCollisionData* pInternalData;		    
};

abstract_class IPhysicsCollisionEvent
{
public:
	virtual void PreCollision(vcollisionevent_t * pEvent) = 0;
	virtual void PostCollision(vcollisionevent_t* pEvent) = 0;

	virtual void Friction(IPhysicsObject* pObject, float energy, int surfaceProps, int surfacePropsHit, IPhysicsCollisionData* pData) = 0;

	virtual void StartTouch(IPhysicsObject* pObject1, IPhysicsObject* pObject2, IPhysicsCollisionData* pTouchData) = 0;
	virtual void EndTouch(IPhysicsObject* pObject1, IPhysicsObject* pObject2, IPhysicsCollisionData* pTouchData) = 0;

	virtual void FluidStartTouch(IPhysicsObject* pObject, IPhysicsFluidController* pFluid) = 0;
	virtual void FluidEndTouch(IPhysicsObject* pObject, IPhysicsFluidController* pFluid) = 0;

	virtual void PostSimulationFrame() = 0;

	virtual void ObjectEnterTrigger(IPhysicsObject* pTrigger, IPhysicsObject* pObject) {}
	virtual void ObjectLeaveTrigger(IPhysicsObject* pTrigger, IPhysicsObject* pObject) {}
};


abstract_class IPhysicsObjectEvent
{
public:
	virtual void ObjectWake(IPhysicsObject * pObject) = 0;
	virtual void ObjectSleep(IPhysicsObject* pObject) = 0;
};

abstract_class IPhysicsConstraintEvent
{
public:
	virtual void ConstraintBroken(IPhysicsConstraint*) = 0;
};

struct hlshadowcontrol_params_t
{
	Vector			targetPosition;
	QAngle			targetRotation;
	float			maxAngular;
	float			maxDampAngular;
	float			maxSpeed;
	float			maxDampSpeed;
	float			dampFactor;
	float			teleportDistance;
};

abstract_class IPhysicsShadowController
{
public:
	virtual ~IPhysicsShadowController(void) {}

	virtual void Update(const Vector & position, const QAngle & angles, float timeOffset) = 0;
	virtual void MaxSpeed(float maxSpeed, float maxAngularSpeed) = 0;
	virtual void StepUp(float height) = 0;

	virtual void SetTeleportDistance(float teleportDistance) = 0;
	virtual bool AllowsTranslation() = 0;
	virtual bool AllowsRotation() = 0;

	virtual void SetPhysicallyControlled(bool isPhysicallyControlled) = 0;
	virtual bool IsPhysicallyControlled() = 0;
	virtual void GetLastImpulse(Vector* pOut) = 0;
	virtual void UseShadowMaterial(bool bUseShadowMaterial) = 0;
	virtual void ObjectMaterialChanged(int materialIndex) = 0;


	virtual float GetTargetPosition(Vector* pPositionOut, QAngle* pAnglesOut) = 0;

	virtual float GetTeleportDistance(void) = 0;
	virtual void GetMaxSpeed(float* pMaxSpeedOut, float* pMaxAngularSpeedOut) = 0;
};

class CPhysicsSimObject;
class IPhysicsMotionController;

class IMotionEvent
{
public:
	enum simresult_e { SIM_NOTHING = 0, SIM_LOCAL_ACCELERATION, SIM_LOCAL_FORCE, SIM_GLOBAL_ACCELERATION, SIM_GLOBAL_FORCE };
	virtual simresult_e	Simulate(IPhysicsMotionController* pController, IPhysicsObject* pObject, float deltaTime, Vector& linear, AngularImpulse& angular) = 0;
};



abstract_class IPhysicsMotionController
{
public:
	virtual ~IPhysicsMotionController(void) {}
	virtual void SetEventHandler(IMotionEvent * handler) = 0;
	virtual void AttachObject(IPhysicsObject* pObject, bool checkIfAlreadyAttached) = 0;
	virtual void DetachObject(IPhysicsObject* pObject) = 0;

	virtual int CountObjects(void) = 0;
	virtual void GetObjects(IPhysicsObject** pObjectList) = 0;
	virtual void ClearObjects(void) = 0;
	virtual void WakeObjects(void) = 0;

	enum priority_t
	{
		LOW_PRIORITY = 0,
		MEDIUM_PRIORITY = 1,
		HIGH_PRIORITY = 2,
	};
	virtual void SetPriority(priority_t priority) = 0;
};

abstract_class IPhysicsCollisionSolver
{
public:
	virtual int ShouldCollide(IPhysicsObject * pObj0, IPhysicsObject * pObj1, void* pGameData0, void* pGameData1) = 0;
	virtual int ShouldSolvePenetration(IPhysicsObject* pObj0, IPhysicsObject* pObj1, void* pGameData0, void* pGameData1, float dt) = 0;

	virtual bool ShouldFreezeObject(IPhysicsObject* pObject) = 0;

	virtual int AdditionalCollisionChecksThisTick(int currentChecksDone) = 0;

	virtual bool ShouldFreezeContacts(IPhysicsObject** pObjectList, int objectCount) = 0;
};

enum PhysicsTraceType_t
{
	VPHYSICS_TRACE_EVERYTHING = 0,
	VPHYSICS_TRACE_STATIC_ONLY,
	VPHYSICS_TRACE_MOVING_ONLY,
	VPHYSICS_TRACE_TRIGGERS_ONLY,
	VPHYSICS_TRACE_STATIC_AND_MOVING,
};

abstract_class IPhysicsTraceFilter
{
public:
	virtual bool ShouldHitObject(IPhysicsObject * pObject, int contentsMask) = 0;
	virtual PhysicsTraceType_t	GetTraceType() const = 0;
};

abstract_class IPhysicsEnvironment
{
public:
	virtual ~IPhysicsEnvironment(void) {}

	virtual void SetDebugOverlay(CreateInterfaceFn debugOverlayFactory) = 0;
	virtual IVPhysicsDebugOverlay* GetDebugOverlay(void) = 0;

	virtual void			SetGravity(const Vector& gravityVector) = 0;
	virtual void			GetGravity(Vector* pGravityVector) const = 0;

	virtual void			SetAirDensity(float density) = 0;
	virtual float			GetAirDensity(void) const = 0;

	virtual IPhysicsObject* CreatePolyObject(const CPhysCollide* pCollisionModel, int materialIndex, const Vector& position, const QAngle& angles, objectparams_t* pParams) = 0;
	virtual IPhysicsObject* CreatePolyObjectStatic(const CPhysCollide* pCollisionModel, int materialIndex, const Vector& position, const QAngle& angles, objectparams_t* pParams) = 0;
	virtual IPhysicsObject* CreateSphereObject(float radius, int materialIndex, const Vector& position, const QAngle& angles, objectparams_t* pParams, bool isStatic) = 0;
	virtual void DestroyObject(IPhysicsObject*) = 0;

	virtual IPhysicsFluidController* CreateFluidController(IPhysicsObject* pFluidObject, fluidparams_t* pParams) = 0;
	virtual void DestroyFluidController(IPhysicsFluidController*) = 0;

	virtual IPhysicsSpring* CreateSpring(IPhysicsObject* pObjectStart, IPhysicsObject* pObjectEnd, springparams_t* pParams) = 0;
	virtual void DestroySpring(IPhysicsSpring*) = 0;

	virtual IPhysicsConstraint* CreateRagdollConstraint(IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_ragdollparams_t& ragdoll) = 0;
	virtual IPhysicsConstraint* CreateHingeConstraint(IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_hingeparams_t& hinge) = 0;
	virtual IPhysicsConstraint* CreateFixedConstraint(IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_fixedparams_t& fixed) = 0;
	virtual IPhysicsConstraint* CreateSlidingConstraint(IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_slidingparams_t& sliding) = 0;
	virtual IPhysicsConstraint* CreateBallsocketConstraint(IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_ballsocketparams_t& ballsocket) = 0;
	virtual IPhysicsConstraint* CreatePulleyConstraint(IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_pulleyparams_t& pulley) = 0;
	virtual IPhysicsConstraint* CreateLengthConstraint(IPhysicsObject* pReferenceObject, IPhysicsObject* pAttachedObject, IPhysicsConstraintGroup* pGroup, const constraint_lengthparams_t& length) = 0;

	virtual void DestroyConstraint(IPhysicsConstraint*) = 0;

	virtual IPhysicsConstraintGroup* CreateConstraintGroup(const constraint_groupparams_t& groupParams) = 0;
	virtual void DestroyConstraintGroup(IPhysicsConstraintGroup* pGroup) = 0;

	virtual IPhysicsShadowController* CreateShadowController(IPhysicsObject* pObject, bool allowTranslation, bool allowRotation) = 0;
	virtual void						DestroyShadowController(IPhysicsShadowController*) = 0;

	virtual IPhysicsPlayerController* CreatePlayerController(IPhysicsObject* pObject) = 0;
	virtual void						DestroyPlayerController(IPhysicsPlayerController*) = 0;

	virtual IPhysicsMotionController* CreateMotionController(IMotionEvent* pHandler) = 0;
	virtual void						DestroyMotionController(IPhysicsMotionController* pController) = 0;

	virtual IPhysicsVehicleController* CreateVehicleController(IPhysicsObject* pVehicleBodyObject, const vehicleparams_t& params, unsigned int nVehicleType, IPhysicsGameTrace* pGameTrace) = 0;
	virtual void						DestroyVehicleController(IPhysicsVehicleController*) = 0;

	virtual void			SetCollisionSolver(IPhysicsCollisionSolver* pSolver) = 0;

	virtual void			Simulate(float deltaTime) = 0;
	virtual bool			IsInSimulation() const = 0;

	virtual float			GetSimulationTimestep() const = 0;
	virtual void			SetSimulationTimestep(float timestep) = 0;

	virtual float			GetSimulationTime() const = 0;
	virtual void			ResetSimulationClock() = 0;
	virtual float			GetNextFrameTime(void) const = 0;

	virtual void			SetCollisionEventHandler(IPhysicsCollisionEvent* pCollisionEvents) = 0;
	virtual void			SetObjectEventHandler(IPhysicsObjectEvent* pObjectEvents) = 0;
	virtual	void			SetConstraintEventHandler(IPhysicsConstraintEvent* pConstraintEvents) = 0;

	virtual void			SetQuickDelete(bool bQuick) = 0;

	virtual int				GetActiveObjectCount() const = 0;
	virtual void			GetActiveObjects(IPhysicsObject** pOutputObjectList) const = 0;
	virtual const IPhysicsObject** GetObjectList(int* pOutputObjectCount) const = 0;
	virtual bool			TransferObject(IPhysicsObject* pObject, IPhysicsEnvironment* pDestinationEnvironment) = 0;

	virtual void			CleanupDeleteList(void) = 0;
	virtual void			EnableDeleteQueue(bool enable) = 0;

	virtual bool			Save(const physsaveparams_t& params) = 0;
	virtual void			PreRestore(const physprerestoreparams_t& params) = 0;
	virtual bool			Restore(const physrestoreparams_t& params) = 0;
	virtual void			PostRestore() = 0;

	virtual bool IsCollisionModelUsed(CPhysCollide* pCollide) const = 0;

	virtual void TraceRay(const Ray_t& ray, unsigned int fMask, IPhysicsTraceFilter* pTraceFilter, trace_t* pTrace) = 0;
	virtual void SweepCollideable(const CPhysCollide* pCollide, const Vector& vecAbsStart, const Vector& vecAbsEnd,
		const QAngle& vecAngles, unsigned int fMask, IPhysicsTraceFilter* pTraceFilter, trace_t* pTrace) = 0;

	virtual void GetPerformanceSettings(physics_performanceparams_t* pOutput) const = 0;
	virtual void SetPerformanceSettings(const physics_performanceparams_t* pSettings) = 0;

	virtual void ReadStats(physics_stats_t* pOutput) = 0;
	virtual void ClearStats() = 0;

	virtual unsigned int	GetObjectSerializeSize(IPhysicsObject* pObject) const = 0;
	virtual void			SerializeObjectToBuffer(IPhysicsObject* pObject, unsigned char* pBuffer, unsigned int bufferSize) = 0;
	virtual IPhysicsObject* UnserializeObjectFromBuffer(void* pGameData, unsigned char* pBuffer, unsigned int bufferSize, bool enableCollisions) = 0;


	virtual void EnableConstraintNotify(bool bEnable) = 0;
	virtual void DebugCheckContacts(void) = 0;
};

enum callbackflags
{
	CALLBACK_GLOBAL_COLLISION = 0x0001,
	CALLBACK_GLOBAL_FRICTION = 0x0002,
	CALLBACK_GLOBAL_TOUCH = 0x0004,
	CALLBACK_GLOBAL_TOUCH_STATIC = 0x0008,
	CALLBACK_SHADOW_COLLISION = 0x0010,
	CALLBACK_GLOBAL_COLLIDE_STATIC = 0x0020,
	CALLBACK_IS_VEHICLE_WHEEL = 0x0040,
	CALLBACK_FLUID_TOUCH = 0x0100,
	CALLBACK_NEVER_DELETED = 0x0200,	             
	CALLBACK_MARKED_FOR_DELETE = 0x0400,	              
											CALLBACK_ENABLING_COLLISION = 0x0800,	           
																					CALLBACK_DO_FLUID_SIMULATION = 0x1000,          
																					CALLBACK_IS_PLAYER_CONTROLLER = 0x2000,	             
																					CALLBACK_CHECK_COLLISION_DISABLE = 0x4000,
																					CALLBACK_MARKED_FOR_TEST = 0x8000,	       
};

abstract_class IPhysicsObject
{
public:
	virtual ~IPhysicsObject(void) {}

	virtual bool			IsStatic() const = 0;
	virtual bool			IsAsleep() const = 0;
	virtual bool			IsTrigger() const = 0;
	virtual bool			IsFluid() const = 0;		               
	virtual bool			IsHinged() const = 0;
	virtual bool			IsCollisionEnabled() const = 0;
	virtual bool			IsGravityEnabled() const = 0;
	virtual bool			IsDragEnabled() const = 0;
	virtual bool			IsMotionEnabled() const = 0;
	virtual bool			IsMoveable() const = 0;	     
	virtual bool			IsAttachedToConstraint(bool bExternalOnly) const = 0;

	virtual void			EnableCollisions(bool enable) = 0;
	virtual void			EnableGravity(bool enable) = 0;
	virtual void			EnableDrag(bool enable) = 0;
	virtual void			EnableMotion(bool enable) = 0;

	virtual void			SetGameData(void* pGameData) = 0;
	virtual void* GetGameData(void) const = 0;
	virtual void			SetGameFlags(unsigned short userFlags) = 0;
	virtual unsigned short	GetGameFlags(void) const = 0;
	virtual void			SetGameIndex(unsigned short gameIndex) = 0;
	virtual unsigned short	GetGameIndex(void) const = 0;

	virtual void			SetCallbackFlags(unsigned short callbackflags) = 0;
	virtual unsigned short	GetCallbackFlags(void) const = 0;

	virtual void			Wake(void) = 0;
	virtual void			Sleep(void) = 0;
	virtual void			RecheckCollisionFilter() = 0;
	virtual void			RecheckContactPoints() = 0;

	virtual void			SetMass(float mass) = 0;
	virtual float			GetMass(void) const = 0;
	virtual float			GetInvMass(void) const = 0;
	virtual Vector			GetInertia(void) const = 0;
	virtual Vector			GetInvInertia(void) const = 0;
	virtual void			SetInertia(const Vector& inertia) = 0;

	virtual void			SetDamping(const float* speed, const float* rot) = 0;
	virtual void			GetDamping(float* speed, float* rot) const = 0;

	virtual void			SetDragCoefficient(float* pDrag, float* pAngularDrag) = 0;
	virtual void			SetBuoyancyRatio(float ratio) = 0;			  

	virtual int				GetMaterialIndex() const = 0;
	virtual void			SetMaterialIndex(int materialIndex) = 0;

	virtual unsigned int	GetContents() const = 0;
	virtual void			SetContents(unsigned int contents) = 0;

	virtual float			GetSphereRadius() const = 0;
	virtual float			GetEnergy() const = 0;
	virtual Vector			GetMassCenterLocalSpace() const = 0;

	virtual void			SetPosition(const Vector& worldPosition, const QAngle& angles, bool isTeleport) = 0;
	virtual void			SetPositionMatrix(const matrix3x4_t& matrix, bool isTeleport) = 0;

	virtual void			GetPosition(Vector* worldPosition, QAngle* angles) const = 0;
	virtual void			GetPositionMatrix(matrix3x4_t* positionMatrix) const = 0;
	virtual void			SetVelocity(const Vector* velocity, const AngularImpulse* angularVelocity) = 0;

	virtual void			SetVelocityInstantaneous(const Vector* velocity, const AngularImpulse* angularVelocity) = 0;

	virtual void			GetVelocity(Vector* velocity, AngularImpulse* angularVelocity) const = 0;

	virtual void			AddVelocity(const Vector* velocity, const AngularImpulse* angularVelocity) = 0;
	virtual void			GetVelocityAtPoint(const Vector& worldPosition, Vector* pVelocity) const = 0;
	virtual void			GetImplicitVelocity(Vector* velocity, AngularImpulse* angularVelocity) const = 0;
	virtual void			LocalToWorld(Vector* worldPosition, const Vector& localPosition) const = 0;
	virtual void			WorldToLocal(Vector* localPosition, const Vector& worldPosition) const = 0;

	virtual void			LocalToWorldVector(Vector* worldVector, const Vector& localVector) const = 0;
	virtual void			WorldToLocalVector(Vector* localVector, const Vector& worldVector) const = 0;

	virtual void			ApplyForceCenter(const Vector& forceVector) = 0;
	virtual void			ApplyForceOffset(const Vector& forceVector, const Vector& worldPosition) = 0;
	virtual void			ApplyTorqueCenter(const AngularImpulse& torque) = 0;

	virtual void			CalculateForceOffset(const Vector& forceVector, const Vector& worldPosition, Vector* centerForce, AngularImpulse* centerTorque) const = 0;
	virtual void			CalculateVelocityOffset(const Vector& forceVector, const Vector& worldPosition, Vector* centerVelocity, AngularImpulse* centerAngularVelocity) const = 0;
	virtual float			CalculateLinearDrag(const Vector& unitDirection) const = 0;
	virtual float			CalculateAngularDrag(const Vector& objectSpaceRotationAxis) const = 0;

	virtual bool			GetContactPoint(Vector* contactPoint, IPhysicsObject** contactObject) const = 0;

	virtual void			SetShadow(float maxSpeed, float maxAngularSpeed, bool allowPhysicsMovement, bool allowPhysicsRotation) = 0;
	virtual void			UpdateShadow(const Vector& targetPosition, const QAngle& targetAngles, bool tempDisableGravity, float timeOffset) = 0;

	virtual int				GetShadowPosition(Vector* position, QAngle* angles) const = 0;
	virtual IPhysicsShadowController* GetShadowController(void) const = 0;
	virtual void			RemoveShadowController() = 0;
	virtual float			ComputeShadowControl(const hlshadowcontrol_params_t& params, float secondsToArrival, float dt) = 0;


	virtual const CPhysCollide* GetCollide(void) const = 0;
	virtual const char* GetName() const = 0;

	virtual void			BecomeTrigger() = 0;
	virtual void			RemoveTrigger() = 0;

	virtual void			BecomeHinged(int localAxis) = 0;
	virtual void			RemoveHinged() = 0;

	virtual IPhysicsFrictionSnapshot* CreateFrictionSnapshot() = 0;
	virtual void DestroyFrictionSnapshot(IPhysicsFrictionSnapshot* pSnapshot) = 0;

	virtual void			OutputDebugInfo() const = 0;

};


abstract_class IPhysicsSpring
{
public:
	virtual ~IPhysicsSpring(void) {}
	virtual void			GetEndpoints(Vector * worldPositionStart, Vector * worldPositionEnd) = 0;
	virtual void			SetSpringConstant(float flSpringContant) = 0;
	virtual void			SetSpringDamping(float flSpringDamping) = 0;
	virtual void			SetSpringLength(float flSpringLenght) = 0;

	virtual IPhysicsObject* GetStartObject(void) = 0;

	virtual IPhysicsObject* GetEndObject(void) = 0;
};


struct surfacephysicsparams_t
{
	float			friction;
	float			elasticity;				         
	float			density;				      
	float			thickness;				         
	float			dampening;
};

struct surfaceaudioparams_t
{
	float			reflectivity;		            
	float			hardnessFactor;	        
	float			roughnessFactor;	        

	float			roughThreshold;	            
	float			hardThreshold;	            
	float			hardVelocityThreshold;	            
};

struct surfacesoundnames_t
{
	unsigned short	stepleft;
	unsigned short	stepright;

	unsigned short	impactSoft;
	unsigned short	impactHard;

	unsigned short	scrapeSmooth;
	unsigned short	scrapeRough;

	unsigned short	bulletImpact;
	unsigned short	rolling;

	unsigned short	breakSound;
	unsigned short	strainSound;
};

struct surfacesoundhandles_t
{
	short	stepleft;
	short	stepright;

	short	impactSoft;
	short	impactHard;

	short	scrapeSmooth;
	short	scrapeRough;

	short	bulletImpact;
	short	rolling;

	short	breakSound;
	short	strainSound;
};

struct surfacegameprops_t
{
	float				flMaxSpeedFactor;
	float				flJumpFactor;
	float				flPenetrationModifier;
	float				flDamageModifier;
	MaterialHandle_t	hMaterial;
	std::byte			dClimbable;
	std::byte			pad0[0x4];
}; // Size: 0x17

/*
struct surfacegameprops_t
{
	float			maxSpeedFactor;			         
	float			jumpFactor;				            
	unsigned short	material;
	unsigned char	climbable;
	unsigned char	pad;
};
*/

struct surfacedata_t
{
	surfacephysicsparams_t	physics;	  
	surfaceaudioparams_t	audio;		  
	surfacesoundnames_t		sounds;		    
	surfacegameprops_t		game;		    

	surfacesoundhandles_t		soundhandles;
};

#define VPHYSICS_SURFACEPROPS_INTERFACE_VERSION	"VPhysicsSurfaceProps001"
abstract_class IPhysicsSurfaceProps
{
public:
	virtual ~IPhysicsSurfaceProps(void) {}

	virtual int		ParseSurfaceData(const char* pFilename, const char* pTextfile) = 0;
	virtual int		SurfacePropCount(void) const = 0;

	virtual int		GetSurfaceIndex(const char* pSurfacePropName) const = 0;
	virtual void	GetPhysicsProperties(int surfaceDataIndex, float* density, float* thickness, float* friction, float* elasticity) const = 0;

	virtual surfacedata_t* GetSurfaceData(int surfaceDataIndex) = 0;
	virtual const char* GetString(unsigned short stringTableIndex) const = 0;


	virtual const char* GetPropName(int surfaceDataIndex) const = 0;

	virtual void	SetWorldMaterialIndexTable(int* pMapArray, int mapSize) = 0;

	virtual void	GetPhysicsParameters(int surfaceDataIndex, surfacephysicsparams_t* pParamsOut) const = 0;
};

abstract_class IPhysicsFluidController
{
public:
	virtual ~IPhysicsFluidController(void) {}

	virtual void	SetGameData(void* pGameData) = 0;
	virtual void* GetGameData(void) const = 0;

	virtual void	GetSurfacePlane(Vector* pNormal, float* pDist) const = 0;
	virtual float	GetDensity() const = 0;
	virtual void	WakeAllSleepingObjects() = 0;
	virtual int		GetContents() const = 0;
};


struct fluidparams_t
{
	Vector4D	surfacePlane;	       
	Vector		currentVelocity;       
	float		damping;		     
	float		torqueFactor;
	float		viscosityFactor;
	void* pGameData;
	bool		useAerodynamics;        
	int			contents;

	fluidparams_t() {}
	fluidparams_t(fluidparams_t const& src)
	{
		Vector4DCopy(src.surfacePlane, surfacePlane);
		VectorCopy(src.currentVelocity, currentVelocity);
		damping = src.damping;
		torqueFactor = src.torqueFactor;
		viscosityFactor = src.viscosityFactor;
		contents = src.contents;
	}
};

struct springparams_t
{
	springparams_t()
	{
		memset(this, 0, sizeof(*this));
	}
	float	constant;		  
	float	naturalLength;  
	float	damping;		  
	float	relativeDamping;	              
	Vector	startPosition;
	Vector	endPosition;
	bool	useLocalPositions;	                 
	bool	onlyStretch;		            
};

struct objectparams_t
{
	Vector* massCenterOverride;
	float		mass;
	float		inertia;
	float		damping;
	float		rotdamping;
	float		rotInertiaLimit;
	const char* pName;				    
	void* pGameData;
	float		volume;
	float		dragCoefficient;
	bool		enableCollisions;
};

struct convertconvexparams_t
{
	bool		buildOuterConvexHull;
	bool		buildDragAxisAreas;
	bool		buildOptimizedTraceTables;
	float		dragAreaEpsilon;
	CPhysConvex* pForcedOuterHull;

	void Defaults()
	{
		dragAreaEpsilon = 0.25f;     
		buildOuterConvexHull = false;
		buildDragAxisAreas = false;
		buildOptimizedTraceTables = false;
		pForcedOuterHull = NULL;
	}
};


struct physsaveparams_t
{
	ISave* pSave;
	void* pObject;
	PhysInterfaceId_t 	type;
};

struct physrestoreparams_t
{
	IRestore* pRestore;
	void** ppObject;
	PhysInterfaceId_t 	type;
	void* pGameData;
	const char* pName;				    
	const CPhysCollide* pCollisionModel;
	IPhysicsEnvironment* pEnvironment;
	IPhysicsGameTrace* pGameTrace;
};

struct physrecreateparams_t
{
	void* pOldObject;
	void* pNewObject;
};

struct physprerestoreparams_t
{
	int recreatedObjectCount;
	physrecreateparams_t recreatedObjectList[1];
};

#define DEFINE_PIID( type, enumval ) \
	template <> inline PhysInterfaceId_t GetPhysIID<type>( type ** ) { return enumval; }

template <class PHYSPTR> inline PhysInterfaceId_t GetPhysIID(PHYSPTR**);        

DEFINE_PIID(IPhysicsObject, PIID_IPHYSICSOBJECT);
DEFINE_PIID(IPhysicsFluidController, PIID_IPHYSICSFLUIDCONTROLLER);
DEFINE_PIID(IPhysicsSpring, PIID_IPHYSICSSPRING);
DEFINE_PIID(IPhysicsConstraintGroup, PIID_IPHYSICSCONSTRAINTGROUP);
DEFINE_PIID(IPhysicsConstraint, PIID_IPHYSICSCONSTRAINT);
DEFINE_PIID(IPhysicsShadowController, PIID_IPHYSICSSHADOWCONTROLLER);
DEFINE_PIID(IPhysicsPlayerController, PIID_IPHYSICSPLAYERCONTROLLER);
DEFINE_PIID(IPhysicsMotionController, PIID_IPHYSICSMOTIONCONTROLLER);
DEFINE_PIID(IPhysicsVehicleController, PIID_IPHYSICSVEHICLECONTROLLER);
DEFINE_PIID(IPhysicsGameTrace, PIID_IPHYSICSGAMETRACE);

#endif  