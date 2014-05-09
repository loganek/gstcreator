/*
 * ILockable.h
 *
 *  Created on: May 9, 2014
 *      Author: loganek
 */

#ifndef ILOCKABLE_H_
#define ILOCKABLE_H_

class ILockable
{
private:
	bool lock_ = false;
protected:
	bool is_locked() { return lock_; }
	friend class TransactionLocker;
};

class TransactionLocker
{
private:
	ILockable* l;
public:
	TransactionLocker(ILockable* l)
	: l(l) { l->lock_ = true; }
	~TransactionLocker() { l->lock_ = false; }
};

#endif /* ILOCKABLE_H_ */
