#include "hfprojectswizzard.h"
#include "textwizardpage.h"
#include "projectswizardpage.h"

HfProjectsWizzard::HfProjectsWizzard() :
    QWizard()
{
    addPage(new TextWizardPage(this));
    addPage(new ProjectsWizardPage(this));
}

HfProjectsWizzard::~HfProjectsWizzard()
{

}
