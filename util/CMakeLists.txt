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

add_library(transformation_replace transformation_replace.h transformation_replace.c)
target_link_libraries(transformation_replace transformation)

add_library(transformation_remove transformation_remove.h transformation_remove.c)
target_link_libraries(transformation_remove transformation)

add_executable(transformation_util_test test.c)
target_link_libraries(transformation_util_test transformation_replace transformation_remove
                      controller source_write sink_read)

add_test(transformation_util_test transformation_util_test)