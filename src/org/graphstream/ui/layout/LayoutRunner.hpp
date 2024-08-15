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

/**
 * @since 2010-01-20
 *
 * @author Guilhelm Savin <guilhelm.savin@graphstream-project.org>
 * @author Antoine Dutot <antoine.dutot@graphstream-project.org>
 * @author Yoann Pigné <yoann.pigne@graphstream-project.org>
 * @author Alex Bowen <bowen.a@gmail.com>
 * @author Hicham Brahimi <hicham.brahimi@graphstream-project.org>
 */

#ifndef LAYOUTRUNNER_HPP
#define LAYOUTRUNNER_HPP

#include <thread>
#include <atomic>
#include <memory>
#include <iostream>
#include "Layout.hpp"
#include "ProxyPipe.hpp"
#include "ThreadProxyPipe.hpp"
#include "Graph.hpp"

class LayoutRunner {
public:
    LayoutRunner(std::shared_ptr<Source> source, std::shared_ptr<Layout> layout, bool startImmediately = true);
    LayoutRunner(std::shared_ptr<Graph> graph, std::shared_ptr<Layout> layout, bool startImmediately = true, bool replay = true);
    ~LayoutRunner();

    std::unique_ptr<ProxyPipe> newLayoutPipe();

    void release();
    void setNaps(long longNap, long shortNap);

private:
    void run();
    void nap(long ms);

    std::shared_ptr<Layout> layout;
    std::shared_ptr<ThreadProxyPipe> pumpPipe;
    std::atomic<bool> loop;
    long longNap;
    long shortNap;
    std::thread layoutThread;
};

#endif // LAYOUTRUNNER_HPP
