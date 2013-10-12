# Sourced from http://stackoverflow.com/a/12196398:

# Expands to comma-separated list of numeric arguments appearing in the
# current call context.
args = \
    $(eval __arg_tmp := \
        $(call __args_strip_tail,$(foreach __a,.1,$(__args_list))))$(__arg_tmp)

# The list of non-empty argument references, e.g. $(1),,$(3),,,
# Context: __a - encoded number of an argument being currently processed.
__args_list = \
    $(if $(__arg_value),$(__arg_ref))$(foreach __a,$(call inc,$(__a)) \
        ,$(if $(__arg_simple),$(comma)$(eval __arg_tmp := \
            $(value __args_list))$(__arg_tmp)))

__arg_value  = $(value $(call to_dec,$(__a)))
__arg_ref    = $${$(call to_dec,$(__a))}
__arg_simple = $(findstring simple,$(flavor $(call to_dec,$(__a))))

# Used as a workaround for GNU Make bugs in 'call'.
# ${1},,${3},,, -> ${1},,${3}
__args_strip_tail = $(subst } ,},$(call nolastword,$(subst },} ,$1,)))

# Some useful stuff...
comma := ,
nolastword = $(wordlist 2,$(words $1),x $1)

# Decodes .4.2 into 42
to_dec = $(subst .,,$1)

# Increments the given numeber.
#   1. Decimal with periods between digits: 42 is .4.2
inc = \
    $(call __inc$(suffix .0$1),$(basename $1))

__inc.0 = $1.1
__inc.1 = $1.2
__inc.2 = $1.3
__inc.3 = $1.4
__inc.4 = $1.5
__inc.5 = $1.6
__inc.6 = $1.7
__inc.7 = $1.8
__inc.8 = $1.9
__inc.9 = $(call inc,$1).0
