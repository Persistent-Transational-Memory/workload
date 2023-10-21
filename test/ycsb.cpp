/*
 * @author: BL-GS 
 * @date:   2022/12/13
 */

#include <fstream>

#include <gtest/internal/gtest-type-util.h>
#include <gtest/gtest.h>

#include <concurrent_control/abstract_concurrent_control.h>
#include <workload/config/ycsb_config.h>
#include <workload/ycsb/ycsb.h>

template<class Key>
class Executor {
public:
	using Self        = Executor<Key>;

	using KeyType     = Key;
	using AbKeyType   = workload::AbstractKey<KeyType>;

private:
	std::ofstream *file_stream_ptr_;

public:
	Executor(std::ofstream &file_stream): file_stream_ptr_(&file_stream) {}

public: // Interface for workload

	bool read(AbKeyType key, void *row_ptr, uint32_t size, uint32_t offset = 0) {
		*file_stream_ptr_ << "Read," << key.logic_key_ << ',' << size << ',' << offset << ',' << 0 << "\n";
		return true;
	}

	bool update(AbKeyType key, const void *row_ptr, uint32_t size, uint32_t offset = 0) {
		*file_stream_ptr_ << "Update," << key.logic_key_ << ',' << size << ',' << offset << ',' << 0 << "\n";
		return true;
	}

	bool scan(AbKeyType key, void *row_ptr, uint32_t size, uint32_t scan_length, uint32_t offset = 0) {
		*file_stream_ptr_ << "Scan," << key.logic_key_ << ',' << size << ',' << offset << ',' << scan_length << "\n";
		return true;
	}

	bool insert(AbKeyType key, const void *row, uint32_t size) {
		*file_stream_ptr_ << "Insert," << key.logic_key_ << ',' << size << ',' << 0 << ',' << 0 << "\n";
		return true;
	}

	bool remove(AbKeyType key) {
		*file_stream_ptr_ << "Remove," << key.logic_key_ << ',' << 0 << ',' << 0 << ',' << 0 << "\n";
		return true;
	}

	bool read_before_modify(AbKeyType key, void *row_ptr, uint32_t size, uint32_t offset = 0) {
		*file_stream_ptr_ << "ReadBeforeModify," << key.logic_key_ << ',' << size << ',' << offset << ",\n";
		return true;
	}
};

template<class TestConfig>
class YCSBTest : public testing::Test {
public:
	using BenchType       = workload::YCSB<TestConfig>;
	using KeyType         = BenchType::KeyType;
	using TransactionType = typename BenchType::Transaction;

private:
	std::string output_file_name_ = "YCSBTest_";

private:
	BenchType ycsb_generator_;

	std::ofstream file_stream_;

public:
	void generate_transaction() {
		TransactionType transaction = ycsb_generator_.generate_transaction();

		Executor<KeyType> executor(file_stream_);
		EXPECT_TRUE(transaction.run(executor));
	}

	void initialize() {
		std::vector<TransactionType> transaction_array{ycsb_generator_.initialize_insert()};
		Executor<KeyType> executor(file_stream_);
		for (auto &transaction: transaction_array) {
			transaction.run(executor);
		}
	}

public:
	void SetUp() override {
		// Build file name
		output_file_name_ += TestConfig::CONFIG_TYPE_NAME;
		output_file_name_ += ".csv";
		// Open file as stream
		file_stream_ = std::ofstream(output_file_name_);
		// Insert column names
		file_stream_ << "Type,Key,Size,Offset,ScanLength\n";
		// Fill the lower level.
		initialize();
	}

	void TearDown() override {
		file_stream_.close();
	}
};


// Register all configuration to be tested.
using TestObjectRegister = ::testing::Types<
		workload::YCSBConfig<workload::YCSBConfigType::A>,
		workload::YCSBConfig<workload::YCSBConfigType::B>,
		workload::YCSBConfig<workload::YCSBConfigType::C>,
		workload::YCSBConfig<workload::YCSBConfigType::D>,
		workload::YCSBConfig<workload::YCSBConfigType::E>,
		workload::YCSBConfig<workload::YCSBConfigType::F>>;


TYPED_TEST_CASE(YCSBTest, TestObjectRegister);

TYPED_TEST(YCSBTest, TransactionGeneration) {
	for (uint32_t i = 0; i < 10000; ++i) {
		this->generate_transaction();
	}
}