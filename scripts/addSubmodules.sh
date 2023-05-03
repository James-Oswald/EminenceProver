
#This script is useful for every time I accidently mess up all the submodules and need to redo them from scratch

#	path = garden/vampire
#	url = https://github.com/vprover/vampire.git
#	shallow = true
#	branch = 2d02e4655e1b08d1ca9ee7c0aade40f59f046460

#Add vampire 4.7 as a shallow submodule
cd ../garden
git submodule add https://github.com/vprover/vampire.git 
git submodule set-branch -b v4.7 garden/vampire
git config -f ../.gitmodules submodule.garden/vampire.shallow true
