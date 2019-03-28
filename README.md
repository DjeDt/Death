# War

# graphical view

```
#===================#
#    header (entry) #---
#===================#   |
#                   #   |
#     Packer   <----#---
#        |          #
#         ----------#---
#===================#   |
#       Text        #   |
#      [binary]     #   |
#=========^=========#   |
#         |         #   |
#       Virus  <----#---
#===================#

```

# Execution flow

```
* decrypt whole infection routine
* look for running antivirus proccess
* select a binary randomly
* inspect binary metadata
* modify binary metadata
* patch binary metadata
* simple metaporphic routine
* write modified content
* crypt and exit
```

# Text view
In order to split the different entities (packer / virus) we decided to use two different method of infection. Pack routine is injected just below the file header. the virus routine is an ElfMaster creation that we implemented.
