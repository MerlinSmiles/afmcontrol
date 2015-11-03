__declspec(dllexport)
bool Capture(int,enum CaptureFlags);
//__declspec(dllexport)
//struct FileName CaptureDir(void);
//__declspec(dllexport)
//bool ComCTS(int);
//__declspec(dllexport)
//struct FileName CurrentFile(void);
//__declspec(dllexport)
//double DataScanAngle(void);
__declspec(dllexport)
double DataScanRate(void);
//__declspec(dllexport)
// error occurs double DataSetpoint(void);
__declspec(dllexport)
int DoFeatureEngage(void);
__declspec(dllexport)
int DoHeightEngage(void);
__declspec(dllexport)
void DoZoomIn(void);
__declspec(dllexport)
void DoZoomOut(void);
__declspec(dllexport)
bool Engage(void);
__declspec(dllexport)
void EraseMessage(void);
__declspec(dllexport)
void FrameDown(void);
__declspec(dllexport)
void FrameUp(void);
__declspec(dllexport)
float GetAspectRatio(void);
__declspec(dllexport)
//struct FileName GetDir(struct FileName &);
//__declspec(dllexport)
//float GetDirectionScanned(void);
//__declspec(dllexport)
//float GetDriveFreq(void);
__declspec(dllexport)
int GetEngageStatus(void);
__declspec(dllexport)
double GetIntegralGain(void);
__declspec(dllexport)
int GetSamplesPerLine(void);
__declspec(dllexport)
float GetScanAngle(void);
__declspec(dllexport)
int GetScanLines(void);
__declspec(dllexport)
float GetScanRate(void);
__declspec(dllexport)
float GetScanSize(void);
__declspec(dllexport)
double GetSetpoint(void);
__declspec(dllexport)
//void GetStartupPath(struct FileName &);
__declspec(dllexport)
float GetXOffset(void);
__declspec(dllexport)
float GetYOffset(void);
__declspec(dllexport)
void ImageMode(void);
__declspec(dllexport)
bool IsCaptureDone(void);
__declspec(dllexport)
bool IsClosedLoop(void);
__declspec(dllexport)
bool IsEngaged(void);
__declspec(dllexport)
bool IsRealTime(void);
__declspec(dllexport)
bool IsTappingMode(void);
__declspec(dllexport)
bool IsTipExchange(void);
__declspec(dllexport)
//struct FileName LastCapturedFileName(void);
//__declspec(dllexport)
//void LithoAbort(void);
//__declspec(dllexport)
//void MacroAbort(char *);
//__declspec(dllexport)
//struct FileName MakeFileName(char *);
//__declspec(dllexport)
//bool ModifyFileName(struct FileName &,char *);
//__declspec(dllexport)
//int NextSite(void *);
//__declspec(dllexport)
//int NextTipSite(void *,struct Coord3D &,struct FileName &,bool &,int &);
//__declspec(dllexport)
//void RealTime(void);
//__declspec(dllexport)
//bool RegisterSite(void *);
__declspec(dllexport)
bool RegisterSite(class SiteHandle);
__declspec(dllexport)
void SaveFile(void);
__declspec(dllexport)
void ScopeMode(void);
__declspec(dllexport)
void SetAFMMode(enum AFMMode);
__declspec(dllexport)
void SetAspectRatio(float);
__declspec(dllexport)
void SetCaptureFileName(struct FileName &);
__declspec(dllexport)
void SetCurrentDir(struct FileName &);
__declspec(dllexport)
void SetExtraSiteInfo(void *,struct ExtraSiteInfo &);
__declspec(dllexport)
void SetIntegralGain(double);
__declspec(dllexport)
void SetInterleaveMode(bool);
__declspec(dllexport)
void SetProportionalGain(double);
__declspec(dllexport)
void SetSamplesPerLine(int);
__declspec(dllexport)
void SetScanAngle(float);
__declspec(dllexport)
void SetScanLines(int);
__declspec(dllexport)
void SetScanRate(float);
__declspec(dllexport)
void SetScanSize(float);
__declspec(dllexport)
void SetScanSizeOffLine(float);
__declspec(dllexport)
void SetSetpoint(double);
__declspec(dllexport)
void SetXOffset(float);
__declspec(dllexport)
void SetYOffset(float);
__declspec(dllexport)
void ShowMessage(char const *,char const *,...);
__declspec(dllexport)
void SiteFileClose(void *,bool);
__declspec(dllexport)
bool SiteFileEdit(void *);
__declspec(dllexport)
int SiteFileIndex(void *);
__declspec(dllexport)
void SiteFileOpen(struct FileName const &);
__declspec(dllexport)
void SiteFileReset(void *);
__declspec(dllexport)
float StageGetMaxSpeed(enum StageAxisCodes);
__declspec(dllexport)
float StageGetPosition(int);
__declspec(dllexport)
float StageGetSpeed(enum StageAxisCodes);
__declspec(dllexport)
// struct Coord3D StageGetXYZ(void);
//__declspec(dllexport)
//int StageGetXYZReference(long &,long &,long &,long &,long &);
//__declspec(dllexport)
//bool StageMove3D(void *,struct Coord3D &,bool);
__declspec(dllexport)
bool StageMoveAxis(int,float,bool,int,float,bool);
__declspec(dllexport)
bool StageMoveSPM(float,bool);
__declspec(dllexport)
void StageMoveTo(void);
__declspec(dllexport)
bool StageMoveToSurfaceFocus(void);
__declspec(dllexport)
bool StageRegister(void *,bool);
__declspec(dllexport)
void StageResetImageCenter(void);
__declspec(dllexport)
void StageSetXYReference(int,long,long,long,long);
__declspec(dllexport)
void StageSetXYZReference(int,long,long,long,long,long);
__declspec(dllexport)
void StageUnload(void);
__declspec(dllexport)
bool StartCapture(int,enum CaptureFlags);
//__declspec(dllexport)
//void StartNewThread(void *)(void));
__declspec(dllexport)
void ThreadAttach(void);
__declspec(dllexport)
void ThreadDetach(void);
__declspec(dllexport)
char TimeString(void);
__declspec(dllexport)
bool WaitForCapture(void);
__declspec(dllexport)
bool Withdraw(void);
__declspec(dllexport)
bool getSecsOnLine(void);
__declspec(dllexport)
void popMessage(struct SDRMSG *,unsigned short *,char *,long);
__declspec(dllexport)
void pushMessage(struct SDRMSG *,unsigned short *,char * *);
__declspec(dllexport)
bool secsActive(void);
__declspec(dllexport)
void setReportLocation(char *,int,int);
__declspec(dllexport)
void takeABreak(void);