/*
 * @author: BL-GS 
 * @date:   2023/6/4
 */

#pragma once
#ifndef PTM_WORKLOAD_TABLE_INFO_H
#define PTM_WORKLOAD_TABLE_INFO_H

#include <initializer_list>
#include <concepts>

namespace workload {

	template<class Info>
	concept TupleInfoConcept = requires {
		Info::TupleSize;
		Info::TupleExpectNumber;
	};

	template<class Info>
	concept TableInfoConcept = requires {
		Info::KeyType;
		Info::AbKeyType;

		Info::TableDefinition;
		requires TupleInfoConcept<typename Info::TableDefinition::value_type>;
		requires std::is_same_v<typename Info::TableDefinition,
								std::initializer_list<typename Info::TableDefinition::value_type>>;
	};

}

#endif //PTM_WORKLOAD_TABLE_INFO_H
