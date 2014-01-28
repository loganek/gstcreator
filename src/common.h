/*
 * gstcreator
 *     common.h
 *
 *  Created on: 28 sty 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

#ifndef COMMON_H_
#define COMMON_H_

#define DRAG_DROP_FORMAT "application/x-DraggedGstBlock-DragAndDrop"

template<typename T, typename Ret, typename ...Args>
static void safe_call(T* object, void (T::* fun)(Args...), Args... args)
{
	if (object != nullptr)
		(object->*fun)(args...);
}

#endif /* COMMON_H_ */
