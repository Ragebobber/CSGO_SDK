#pragma once

class IBaseClientDll
{
public:
    virtual int              connect(void) = 0;//(create_interface_fn appSystemFactory, c_global_vars_base* pGlobals) = 0;
    virtual int              disconnect(void) = 0;
    virtual int              init(void) = 0; //(create_interface_fn appSystemFactory, c_global_vars_base * pGlobals) = 0;
    virtual void             post_init() = 0;
    virtual void             shutdown(void) = 0;
    virtual void             level_init_pre_entity(char const* pMapName) = 0;
    virtual void             level_init_post_entity() = 0;
    virtual void             level_shutdown(void) = 0;
    virtual ClientClass*     GetAllClasses(void) = 0;
};

