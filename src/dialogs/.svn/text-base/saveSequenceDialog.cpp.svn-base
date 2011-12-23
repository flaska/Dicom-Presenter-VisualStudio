#include <dialogs/saveSequenceDialog.h>
#include <glObjects/workspaces/glWorkspace.h>
#include <glObjects/workspaces/workspaceManager.h>
#include <animation/animationManager.h>

SaveSequenceDialog::SaveSequenceDialog(QWidget *parent, Qt::WFlags flags ):
QDialog(parent, flags)
{
	ui.setupUi(this);
	ui.spinWidth->setMaximum(2028);
	ui.spinHeight->setMaximum(2028);
	iDoDimensionUpdate = true;
	if(CWorkspaceManager::GetInstance())
	{
		if(CWorkspaceManager::GetInstance()->GetActiveWorkspace())
		{
			QRect workspaceInnerRect = CWorkspaceManager::GetInstance()->GetActiveWorkspace()->GetInnerRect();
			ui.spinWidth->setValue(workspaceInnerRect.width());
			QObject::connect(ui.spinWidth,SIGNAL(valueChanged(int)),
				this, SLOT(WidthChanged(int)));
				
			ui.spinHeight->setValue(workspaceInnerRect.height());
						QObject::connect(ui.spinHeight,SIGNAL(valueChanged(int)),
				this, SLOT(HeightChanged(int)));
			iAspectRatio = (float)(workspaceInnerRect.width())/(float)(workspaceInnerRect.height());
			ui.doubleSpinFPS->setValue(CAnimationManager::GetInstance()->GetSaveSequenceFPS());
		}
	}
}

SaveSequenceDialog::~SaveSequenceDialog()
{}


void SaveSequenceDialog::accept()
{
	if(CWorkspaceManager::GetInstance())
	{
		if(CWorkspaceManager::GetInstance()->GetActiveWorkspace())
		{
			if(CAnimationManager::GetInstance())
			{
				CAnimationManager::GetInstance()->SetSaveSequenceFPS(ui.doubleSpinFPS->value());
				CAnimationManager::GetInstance()->SetSaveSequenceKeepFrames(ui.checkBoxKeepFrames->isChecked());
			}
			if(CWorkspaceManager::GetInstance())
			{
				if(CWorkspaceManager::GetInstance()->GetActiveWorkspace())
				{
					CWorkspaceManager::GetInstance()->GetActiveWorkspace()->SetSaveSnapshotHeight(ui.spinHeight->value());
					CWorkspaceManager::GetInstance()->GetActiveWorkspace()->SetSaveSnapshotWidth(ui.spinWidth->value());
				}
			}
		}
	}
	QDialog::accept();
}

void SaveSequenceDialog::HeightChanged(int newValue)
{
	
	if(ui.checkBoxPreserve->isChecked() && iDoDimensionUpdate)
	{
		float newWidth = iAspectRatio*(float)newValue;
		iDoDimensionUpdate = false;
		ui.spinWidth->setValue(newWidth);
		iDoDimensionUpdate = true;
	}
}
void SaveSequenceDialog::WidthChanged(int newValue)
{
	if(ui.checkBoxPreserve->isChecked() && iDoDimensionUpdate)
	{
		float newHeight = (float)newValue /iAspectRatio;
		iDoDimensionUpdate = false;
		ui.spinHeight->setValue(newHeight);
		iDoDimensionUpdate = true;
	}
}
