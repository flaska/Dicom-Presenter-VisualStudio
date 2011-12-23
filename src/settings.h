#ifndef _SETTINGS_H_SADASWDFNMKIO
#define _SETTINGS_H_SADASWDFNMKIO

typedef struct 
{
	float r,g,b,a;
}TColor;

typedef struct
{
	int left, top, right, bottom;
}TBorders;

typedef enum
{
	EWorkspaceTextureWidth,
	EWorkspaceTextureHeight,
	EInfoPanelSpacing,
	EInfoPanelMarginLeft,
	EInfoPanelMarginTop,
	EInfoPanelMarginRight,
	EInfoPanelMarginBottom,
	EMaximumFramesToLoad,
	EImageBorders,
	EImageBorderColor,
	EImageInnerColor,
	EImageExplorerBorders,
	EImageExplorerBorderColor,
	EImageExplorerInnerColor,
	EWorkspaceExplorerBorders,
	EWorkspaceExplorerBorderColor,
	EWorkspaceExplorerInnerColor,
	EWorkspaceBorders,
	EWorkspaceBorderColor,
	EWorkspaceInnerColor,
EWorkspaceSnapshotBorders,
	EWorkspaceSnapshotBorderColor,
	EWorkspaceSnapshotInnerColor,
	EWorkspaceExplorerTextColor,
	EImageExplorerTextColor
}TSettingConstants;
class Settings
{
public:
	~Settings();
	static void ReadSettings();
	static void WriteSettings();

	static int GetIntegerConstant(TSettingConstants constant);
	static TBorders GetBordersConstant(TSettingConstants constant);
	static TColor GetColorConstant(TSettingConstants constant);
protected:
	Settings();
private:
	static int KWorkspaceTextureWidth; //must be 2^n ?
	static int KWorkspaceTextureHeight;
	static int KInfoPanelSpacing ;
	static int KInfoPanelMarginLeft;
	static int KInfoPanelMarginTop;
	static int KInfoPanelMarginRight;
	static int KInfoPanelMarginBottom;
	static int KMaximumFramesToLoad;

	static TBorders KImageBorders;
	static TColor KImageBorderColor;
	static TColor KImageInnerColor;

	static TBorders KImageExplorerBorders;
	static TColor KImageExplorerBorderColor;
	static TColor KImageExplorerInnerColor;
	static TColor KImageExplorerTextColor;

	static TBorders KWorkspaceBorders;
	static TColor KWorkspaceBorderColor;
	static TColor KWorkspaceInnerColor;

	static TBorders KWorkspaceExplorerBorders;
	static TColor KWorkspaceExplorerBorderColor;
	static TColor KWorkspaceExplorerInnerColor;
	static TColor KWorkspaceExplorerTextColor;

		static TBorders KWorkspaceSnapshotBorders;
	static TColor KWorkspaceSnapshotBorderColor;
	static TColor KWorkspaceSnapshotInnerColor;

	static bool iDeleted;
	static bool iSettingsRead;
	static Settings *instance;
};
#endif

