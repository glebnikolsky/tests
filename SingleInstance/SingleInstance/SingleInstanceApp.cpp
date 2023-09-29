#include "stdafx.h"
#include "SingleInstanceApp.h"
#include <string>

#include <boost/interprocess/sync/named_mutex.hpp>

using namespace boost::interprocess;
using namespace std;

class SingleAppImpl{
public:
	SingleAppImpl(TCHAR* nm): mtx_name_( nm ), locked_this_instance_( false )
		, named_mutex_( new named_mutex( boost::interprocess::open_or_create, mtx_name_.c_str() ) ){}
	~SingleAppImpl(){
		if ( locked_this_instance_ ) named_mutex::remove( mtx_name_.c_str() );
	}
	bool IsAlredyRunningThisMachine(){
		locked_this_instance_ = named_mutex_->try_lock();
		return !locked_this_instance_;
	}
	void RemoveAnyway(){
		named_mutex::remove( mtx_name_.c_str() );
		named_mutex_ = std::shared_ptr<named_mutex>( new named_mutex( boost::interprocess::open_or_create, mtx_name_.c_str() ) );
		locked_this_instance_ = named_mutex_->try_lock();
	}
protected:
	string mtx_name_;
	bool locked_this_instance_;
	std::shared_ptr<named_mutex> named_mutex_;
};


SingleApp::SingleApp(TCHAR* name)
{
	impl_ = std::make_shared<SingleAppImpl>( name );
}

bool SingleApp::IsAlredyRunningThisMachine()
{
	return impl_->IsAlredyRunningThisMachine();
}

void SingleApp::RemoveAnyway()
{
	impl_->RemoveAnyway();
}
