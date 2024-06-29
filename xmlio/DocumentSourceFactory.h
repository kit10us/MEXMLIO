/*
 * QXML Interop Library
 * https://github.com/kit10us/qxmlinterop
 * Copyright (c) 2002, Kit10 Studios LLC
 *
 * This file is part of QXML Interop Library (a.k.a. QXMLIO)
 *
 * QXMLIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QXMLIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QXMLIO.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <io/Source.h>
#include <rm/ResourceManager.h>
#include <me/game/IGame.h>

namespace xmlio
{
	class DocumentSourceFactory : public rm::ISourceFactory< io::Source >
	{
	public:
		typedef std::shared_ptr< rm::ISourceFactory< io::Source> > ptr;

		DocumentSourceFactory(me::game::IGame* game);

		io::Source::ptr Produce(unify::Path source, unify::Parameters parameters) override;
		io::Source::ptr Produce(unify::Parameters parameters) override;

	private:
		me::game::IGame* m_game;
	};
}