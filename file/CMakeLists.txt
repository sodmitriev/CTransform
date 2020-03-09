# Copyright (C) 2020 Sviatoslav Dmitriev
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

add_library(source_file source_file.h source_file.c)
target_link_libraries(source_file source)

add_library(sink_file sink_file.h sink_file.c)
target_link_libraries(sink_file sink)

add_executable(file_test test.c)
target_link_libraries(file_test source_file sink_file controller source_write sink_read)

add_test(file_test file_test)