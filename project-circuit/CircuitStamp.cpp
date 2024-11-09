#include "CircuitStamp.h"

int getRealNode(int node, int ground) {
    if (node <= ground)
        return node;

    return node - 1;
}

void Resistor::contribute(AugmentedMatrix<Number>& a, int ground) {
    if (m_nodeI != ground && m_nodeJ != ground) {
        int workingNodeI = getRealNode(m_nodeI, ground);
        int workingNodeJ = getRealNode(m_nodeJ, ground);

        a.get(workingNodeI, workingNodeI) += 1 / m_value;
        a.get(workingNodeJ, workingNodeJ) += 1 / m_value;
        a.get(workingNodeI, workingNodeJ) += -1 / m_value;
        a.get(workingNodeJ, workingNodeI) += -1 / m_value;

        return;
    }

    int workingNode = getRealNode(m_nodeI != ground ? m_nodeI : m_nodeJ, ground);

    a.get(workingNode, workingNode) += 1 / m_value;
};



void CurrentSource::contribute(AugmentedMatrix<Number>& a, int ground) {
    if (m_nodeI != ground && m_nodeJ != ground) {
        int workingNodeI = getRealNode(m_nodeI, ground);
        int workingNodeJ = getRealNode(m_nodeJ, ground);

        a.get(workingNodeJ, a.columns() - 1) += m_value;
        a.get(workingNodeI, a.columns() - 1) += -m_value;

        return;
    }


    int workingNode = getRealNode(m_nodeI != ground ? m_nodeI : m_nodeJ, ground);

    a.get(workingNode, a.columns() - 1) += (m_nodeJ != ground ? 1 : -1) * m_value;
}


void VoltageSource::contribute(AugmentedMatrix<Number>& a, int ground) {
    a.get(m_voltageRow, a.columns() - 1) = m_value;

    if (m_nodeI != ground && m_nodeJ != ground) {
        int workingNodeI = getRealNode(m_nodeI, ground);
        int workingNodeJ = getRealNode(m_nodeJ, ground);

        a.get(m_voltageRow, workingNodeI) = 1;
        a.get(m_voltageRow, workingNodeJ) = -1;

        a.get(workingNodeI, m_voltageRow) = 1;
        a.get(workingNodeJ, m_voltageRow) = -1;

        a.get(m_voltageRow, a.columns() - 1) = m_value;

        return;
    }

    int workingNode = getRealNode(m_nodeI != ground ? m_nodeI : m_nodeJ, ground);

    a.get(m_voltageRow, workingNode) = 1;
    a.get(workingNode, m_voltageRow) = 1;
}

