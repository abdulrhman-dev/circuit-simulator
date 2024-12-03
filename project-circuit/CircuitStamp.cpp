#include "CircuitStamp.h"

void Resistor::contribute(AugmentedMatrix<Number>& a) {
    if (m_nodeI.isGround && m_nodeJ.isGround)
        return; 


    if (!m_nodeI.isGround && !m_nodeJ.isGround) {

        a.get(m_nodeI.index, m_nodeI.index) += 1 / m_value;
        a.get(m_nodeJ.index, m_nodeJ.index) += 1 / m_value;
        a.get(m_nodeI.index, m_nodeJ.index) += -1 / m_value;
        a.get(m_nodeJ.index, m_nodeI.index) += -1 / m_value;

        return;
    }

    int workingNode = m_nodeI.isGround ? m_nodeJ.index : m_nodeI.index;

    a.get(workingNode, workingNode) += 1 / m_value;
};



void CurrentSource::contribute(AugmentedMatrix<Number>& a) {
    if (m_nodeI.isGround && m_nodeJ.isGround)
        return;

    if (!m_nodeI.isGround && !m_nodeJ.isGround) {
        a.get(m_nodeJ.index, a.columns() - 1) += m_value;
        a.get(m_nodeI.index, a.columns() - 1) += -m_value;

        return;
    }


    int workingNode = m_nodeI.isGround ? m_nodeJ.index : m_nodeI.index;

    a.get(workingNode, a.columns() - 1) += (!m_nodeJ.isGround ? 1 : -1) * m_value;
}


void VoltageSource::contribute(AugmentedMatrix<Number>& a) {
    if (m_nodeI.isGround && m_nodeJ.isGround)
        return;

    a.get(m_voltageRow, a.columns() - 1) = m_value;

    if (!m_nodeI.isGround && !m_nodeJ.isGround) {

        a.get(m_voltageRow, m_nodeI.index) = 1;
        a.get(m_voltageRow, m_nodeJ.index) = -1;

        a.get(m_nodeI.index, m_voltageRow) = 1;
        a.get(m_nodeJ.index, m_voltageRow) = -1;

        return;
    }

    int workingNode = m_nodeI.isGround ? m_nodeJ.index : m_nodeI.index;

    a.get(m_voltageRow, workingNode) = m_nodeJ.isGround ? 1 : -1;
    a.get(workingNode, m_voltageRow) = m_nodeJ.isGround ? 1 : -1;
}

