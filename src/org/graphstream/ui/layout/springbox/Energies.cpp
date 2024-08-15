/*
 * This file is part of GraphStream <http://graphstream-project.org>.
 *
 * GraphStream is a library whose purpose is to handle static or dynamic
 * graphs, create them from scratch, file, or any source, and display them.
 *
 * This program is free software distributed under the terms of two licenses, the
 * CeCILL-C license that fits European law, and the GNU Lesser General Public
 * License. You can use, modify and/or redistribute the software under the terms
 * of the CeCILL-C license as circulated by CEA, CNRS, and INRIA at the following
 * URL <http://www.cecill.info> or under the terms of the GNU LGPL as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL-C and LGPL licenses and that you accept their terms.
 */

#include "Energies.hpp"
#include <cstdlib>

Energies::Energies()
    : energy(0), lastEnergy(0), energiesBuffer(256), energies(256, 0), energiesPos(0), energySum(0) {
    clearEnergies();
}

double Energies::getEnergy() const {
    return lastEnergy;
}

int Energies::getBufferSize() const {
    return energiesBuffer;
}

double Energies::getStabilization() const {
    int range = 200;
    double eprev1 = getPreviousEnergyValue(range);
    double eprev2 = getPreviousEnergyValue(range - 10);
    double eprev3 = getPreviousEnergyValue(range - 20);
    double eprev = (eprev1 + eprev2 + eprev3) / 3.0;
    double diff = std::abs(lastEnergy - eprev);

    diff = diff < 1 ? 1 : diff;

    return 1.0 / diff;
}

double Energies::getAverageEnergy() const {
    return energySum / energies.size();
}

double Energies::getPreviousEnergyValue(int stepsBack) const {
    if (stepsBack >= energies.size())
        stepsBack = energies.size() - 1;

    int pos = (energies.size() + (energiesPos - stepsBack)) % energies.size();
    return energies[pos];
}

void Energies::accumulateEnergy(double value) {
    energy += value;
}

void Energies::storeEnergy() {
    energiesPos = (energiesPos + 1) % energies.size();

    energySum -= energies[energiesPos];
    energies[energiesPos] = energy;
    energySum += energy;
    lastEnergy = energy;
    energy = 0;
}

void Energies::clearEnergies() {
    for (auto &e : energies) {
        e = ((std::rand() / (double)RAND_MAX) * 2000.0) - 1000.0;
    }
}
