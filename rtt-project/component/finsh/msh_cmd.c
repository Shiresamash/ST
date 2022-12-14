/*
 *  internal commands for RT-Thread module shell
 *
 * COPYRIGHT (C) 2013, Shanghai Real-Thread Technology Co., Ltd
 *
 *  This file is part of RT-Thread (http://www.rt-thread.org)
 *  Maintainer: bernard.xiong <bernard.xiong at gmail.com>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-03-30     Bernard      the first verion for FinSH
 */

#include <rtthread.h>
#include <finsh.h>

#include "msh.h"

#ifdef FINSH_USING_MSH
#ifdef RT_USING_DFS
#include <dfs_posix.h>

#ifdef DFS_USING_WORKDIR
extern char working_directory[];
#endif

int cmd_ls(int argc, char** argv)
{
    extern void ls(const char *pathname);

    if (argc == 1)
    {
#ifdef DFS_USING_WORKDIR
        ls(working_directory);
#else
        ls("/");
#endif
    }
    else 
    {
        ls(argv[1]);
    }

    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_ls, __cmd_ls, List information about the FILEs.);

int cmd_cp(int argc, char** argv)
{
    void copy(const char *src, const char *dst);

    if (argc != 3)
    {
        rt_kprintf("Usage: cp SOURCE DEST\n");
        rt_kprintf("Copy SOURCE to DEST.\n");
    }
    else
    {
        copy(argv[1], argv[2]);
    }

    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_cp, __cmd_cp, Copy SOURCE to DEST.);

int cmd_mv(int argc, char** argv)
{
    if (argc != 3)
    {
        rt_kprintf("Usage: mv SOURCE DEST\n");
        rt_kprintf("Rename SOURCE to DEST, or move SOURCE(s) to DIRECTORY.\n");
    }
    else
    {
		int fd;
		char *dest = RT_NULL;

        rt_kprintf("%s => %s\n", argv[1], argv[2]);

		fd = open(argv[2], O_DIRECTORY, 0);
		if (fd >= 0)
		{
			char *src;
			
			close(fd);

			/* it's a directory */			
			dest = (char*)rt_malloc(DFS_PATH_MAX);
			if (dest == RT_NULL)
			{
				rt_kprintf("out of memory\n");
				return -RT_ENOMEM;
			}

			src = argv[1] + rt_strlen(argv[1]);
			while (src != argv[1]) 
			{
				if (*src == '/') break;
				src --;
			}

			rt_snprintf(dest, DFS_PATH_MAX - 1, "%s/%s", argv[2], src);
		}
		else
		{
			fd = open(argv[2], O_RDONLY, 0);
			if (fd >= 0)
			{
				close(fd);
				
				unlink(argv[2]);
			}

			dest = argv[2];
		}

		rename(argv[1], dest);
		if (dest != RT_NULL && dest != argv[2]) rt_free(dest);
    }

    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_mv, __cmd_mv, Rename SOURCE to DEST.);

int cmd_cat(int argc, char** argv)
{
    int index;
    extern void cat(const char* filename);

    if (argc == 1)
    {
        rt_kprintf("Usage: cat [FILE]...\n");
        rt_kprintf("Concatenate FILE(s)\n");
		return 0;
    }

    for (index = 1; index < argc; index ++)
    {
        cat(argv[index]);
    }

    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_cat, __cmd_cat, Concatenate FILE(s));

int cmd_rm(int argc, char** argv)
{
    int index;

    if (argc == 1)
    {
        rt_kprintf("Usage: rm FILE...\n");
        rt_kprintf("Remove (unlink) the FILE(s).\n");
		return 0;
    }

    for (index = 1; index < argc; index ++)
    {
        unlink(argv[index]);
    }

    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_rm, __cmd_rm, Remove (unlink) the FILE(s).);

#ifdef DFS_USING_WORKDIR
int cmd_cd(int argc, char** argv)
{
    if (argc == 1)
    {
        rt_kprintf("%s\n", working_directory);
    }
    else if (argc == 2)
    {
        chdir(argv[1]);
    }

    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_cd, __cmd_cd, Change the shell working directory.);

int cmd_pwd(int argc, char** argv)
{
	rt_kprintf("%s\n", working_directory);
	return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_pwd, __cmd_pwd, Print the name of the current working directory.);
#endif

int cmd_mkdir(int argc, char** argv)
{
	if (argc == 1)
	{
		rt_kprintf("Usage: mkdir [OPTION] DIRECTORY\n");
		rt_kprintf("Create the DIRECTORY, if they do not already exist.\n");
	}
	else
	{
		mkdir(argv[1], 0);
	}

	return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_mkdir, __cmd_mkdir, Create the DIRECTORY.);

#endif

#ifdef RT_USING_LWIP
int cmd_ifconfig(int argc, char** argv)
{
    extern void list_if(void);
    extern void set_if(char* netif_name, char* ip_addr, char* gw_addr, char* nm_addr);


    if(argc == 1)
    {
        list_if();
    }
    else if(argc == 5)
    {
        rt_kprintf("config : %s\n", argv[1]);
        rt_kprintf("IP addr: %s\n", argv[2]);
        rt_kprintf("Gateway: %s\n", argv[3]);
        rt_kprintf("netmask: %s\n", argv[4]);
        set_if(argv[1], argv[2], argv[3], argv[4]);
    }
    else
    {
        rt_kprintf("bad parameter! e.g: ifconfig e0 192.168.1.30 192.168.1.1 255.255.255.0\n");
    }

    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_ifconfig, __cmd_ifconfig, list the information of network interfaces);

#ifdef RT_LWIP_TCP
int cmd_netstat(int argc, char** argv)
{
 	extern void list_tcps(void);

	list_tcps();
	return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_netstat, __cmd_netstat, list the information of TCP/IP);
#endif
#endif /* RT_USING_LWIP */

int cmd_ps(int argc, char** argv)
{
    extern long list_thread(void);

    list_thread();
    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_ps, __cmd_ps, List threads in the system.);

int cmd_time(int argc, char** argv)
{
    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_time, __cmd_time, Execute command with time.);

int cmd_free(int argc, char** argv)
{
    extern void list_mem(void);

    list_mem();
    return 0;
}
FINSH_FUNCTION_EXPORT_ALIAS(cmd_free, __cmd_free, Show the memory usage in the system.);
#endif

