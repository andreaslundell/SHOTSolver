/**
   The Supporting Hyperplane Optimization Toolkit (SHOT).

   @author Andreas Lundell, Åbo Akademi University

   @section LICENSE 
   This software is licensed under the Eclipse Public License 2.0. 
   Please see the README and LICENSE files for more information.
*/

#include "NLPSolverBase.h"

void NLPSolverBase::setProblem(OSInstance *origInstance)
{
    originalInstance = origInstance;
    isProblemInitialized = false;
}

void NLPSolverBase::initializeProblem()
{
    if (!isProblemInitialized)
    {
        createProblemInstance(originalInstance);

        isProblemInitialized = true;
    }
}

void NLPSolverBase::saveProblemToFile(std::string fileName)
{
    if (!isProblemInitialized)
        initializeProblem();

    NLPProblem->saveProblemModelToFile(fileName);
}

E_NLPSolutionStatus NLPSolverBase::solveProblem()
{
    if (!isProblemInitialized)
        initializeProblem();

    if (Settings::getInstance().getBoolSetting("FixedInteger.UsePresolveBounds", "Primal")) // Does not seem to work with Ipopt...
    {
        auto numVar = ProcessInfo::getInstance().originalProblem->getNumberOfVariables();

        for (int i = 0; i < numVar; i++)
        {
            if (i == ProcessInfo::getInstance().originalProblem->getNonlinearObjectiveVariableIdx())
                continue;

            if (ProcessInfo::getInstance().originalProblem->hasVariableBoundsBeenTightened(i))
            {
                NLPProblem->setVariableLowerBound(i,
                                                  ProcessInfo::getInstance().originalProblem->getVariableLowerBound(i));
                NLPProblem->setVariableUpperBound(i,
                                                  ProcessInfo::getInstance().originalProblem->getVariableUpperBound(i));
                NLPProblem->setVariableBoundsAsTightened(i);
            }
        }
    }

    auto solStatus = solveProblemInstance();

    return (solStatus);
}

std::vector<double> NLPSolverBase::getVariableLowerBounds()
{

    if (!isProblemInitialized)
        initializeProblem();

    return (getCurrentVariableLowerBounds());
}

std::vector<double> NLPSolverBase::getVariableUpperBounds()
{
    if (!isProblemInitialized)
        initializeProblem();

    return (getCurrentVariableUpperBounds());
}
