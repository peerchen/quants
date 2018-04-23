#!/bin/sh
#**********************************************#
# 自动化编译脚本
# Author: xujun
# Date: 2016-09-06

# 参数说明 #
# -r : rebuild，指示脚本将全部模块重新编译
# -p : pack up, 表示编译完后进行打包。打包内容包
#          括程序运行环境，以及qtstudio客户端。
# -noup : 不更新代码直接编译
#***********************************************#

if [ "$1" == "-r" -o "$2" == "-r" ]; then
        echo -e "\033[31m即将重新编译...\033[0m"
fi

currpath=`pwd`
if [ "$1" == "-noup" -o "$2" == "-noup" -o "$3" == "-noup" ]; then
        echo -e "\033[31m不更新代码\033[0m"
else
        cd ../../
        echo "cd `pwd`"
        echo -e "\033[35msvn更新代码...\033[0m"

        echo -e "\n\033[33m更新cifox...\033[0m"
        svn up cifox

        echo -e "\n\033[33m更新quants...\033[0m"
        svn up quant/product
        svn up quant/quants
fi
NEEDSVN=1
DEBUG=0

cd $currpath
eval $(svn info | grep -E 'Revision:|版本:' | awk 'NR == 1 {printf("svnversion=%s", $2)}')
echo "svnversion:$svnversion"

release=$currpath/../product/quants/bin
echo $release

if [ "$1" == "-r" -o "$2" == "-r" ]; then
        echo -e "\033[32m清空release/*\033[0m"
	rm $release/*
fi

d_modules="\
	kbarmanager
	tickmanager
	"
for var in $d_modules; do
        cd $var
        echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        pwd
        echo -e "\033[32m--->正在编译$var\033[0m"
    rm -f $release/$var.*
        if [ "$1" == "-r" -o "$2" == "-r" ]; then
                make clean
        fi
	if [ "$DEBUG" == 0 ]; then
		make
	else
		make -e DEBUG=1
	fi
	if [ $? -eq 0 ]; then
		mv $release/$var $release/$var.$svnversion
		ln -s $var.$svnversion $var
		echo -e "\033[32m--->$var编译结束...\033[0m"
	else
        	echo -e "\033[31m--->$var编译错误！\033[0m"
		exit
	fi
        cd ..
done

	cd thostapi 
        echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
        pwd
        echo -e "\033[32m--->正在编译thostapi\033[0m"
    	rm -f $release/../lib/thostapi.*
        if [ "$1" == "-r" -o "$2" == "-r" ]; then
                make clean
        fi
        if [ "$DEBUG" == 0 ]; then
                make
        else
                make -e DEBUG=1
        fi
        if [ $? -eq 0 ]; then
                mv $release/../lib/thostapi.so $release/../lib/thostapi.so.$svnversion
		ln -s thostapi.so.$svnversion thostapi.so
                echo -e "\033[32m--->thostapi编译结束...\033[0m"
        else
                echo -e "\033[31m--->thostapi编译错误！\033[0m"
                exit
        fi
        cd ..

# 打包quants 分布式 release版本

date=`date '+%Y%m%d'`
if [ "$2" == "-p" -o "$1" == "-p" -o "$3" == "-p" ]; then

        echo -e "\033[31m\n\n开始打包...\033[0m"

        if [ -e "$HOME/quants_${date}_${svnversion}.tar.gz" ];then
                rm $HOME/quants_release_${date}_${svnversion}.tar.gz
        fi

        cd ../product/quants/
	echo -e "\033[31m\n\n拷贝boost库...\033[0m"
        cp -d ../../..//boost_1_60_0/stage/linux/lib/*.so* lib/
        echo -e "\033[31m\n\n拷贝依赖库...\033[0m"
	cp -d ../../../cifox/lib/*.so* lib/
	mkdir scripts
        echo -e "\033[31m\n\n拷贝脚本...\033[0m"
	cp ../../quants/scripts/* scripts/
	mkdir config
        echo -e "\033[31m\n\n拷贝配置文件...\033[0m"
	cp ../../quants/config/* config/
        echo "路径：`pwd`"
	#打包
	tar -zcvf $HOME/quants_release_${date}_${svnversion}.tar.gz *
	#zip -r $HOME/quants_release_${date}_${svnversion}.zip ./bin/kbarmanager.$svnversion ./scripts/* ./lib/* ./config/*
	rm -rf scripts
	rm -rf config
fi

echo -e "\n\033[31mAll Done.\033[0m"


