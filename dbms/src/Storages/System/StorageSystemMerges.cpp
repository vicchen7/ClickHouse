#include <Storages/System/StorageSystemMerges.h>
#include <DataTypes/DataTypeString.h>
#include <DataTypes/DataTypesNumber.h>
#include <DataTypes/DataTypeArray.h>
#include <Columns/ColumnString.h>
#include <DataStreams/OneBlockInputStream.h>
#include <Interpreters/Context.h>
#include <Storages/MergeTree/MergeList.h>


namespace DB
{

StorageSystemMerges::StorageSystemMerges(const std::string & name)
    : name{name}
{
    columns = NamesAndTypesList{
        { "database",                    std::make_shared<DataTypeString>() },
        { "table",                       std::make_shared<DataTypeString>() },
        { "elapsed",                     std::make_shared<DataTypeFloat64>() },
        { "progress",                    std::make_shared<DataTypeFloat64>() },
        { "num_parts",                   std::make_shared<DataTypeUInt64>() },
        { "source_part_names",           std::make_shared<DataTypeArray>(std::make_shared<DataTypeString>()) },
        { "result_part_name",            std::make_shared<DataTypeString>() },
        { "total_size_bytes_compressed", std::make_shared<DataTypeUInt64>() },
        { "total_size_marks",            std::make_shared<DataTypeUInt64>() },
        { "bytes_read_uncompressed",     std::make_shared<DataTypeUInt64>() },
        { "rows_read",                   std::make_shared<DataTypeUInt64>() },
        { "bytes_written_uncompressed",  std::make_shared<DataTypeUInt64>() },
        { "rows_written",                std::make_shared<DataTypeUInt64>() },
        { "columns_written",             std::make_shared<DataTypeUInt64>() },
        { "memory_usage",                std::make_shared<DataTypeUInt64>() },
        { "thread_number",               std::make_shared<DataTypeUInt64>() },
    };
}


BlockInputStreams StorageSystemMerges::read(
    const Names & column_names,
    const SelectQueryInfo &,
    const Context & context,
    QueryProcessingStage::Enum & processed_stage,
    const size_t,
    const unsigned)
{
    check(column_names);
    processed_stage = QueryProcessingStage::FetchColumns;

    MutableColumns res_columns = getSampleBlock().cloneEmptyColumns();

    for (const auto & merge : context.getMergeList().get())
    {
        size_t i = 0;
        res_columns[i++]->insert(merge.database);
        res_columns[i++]->insert(merge.table);
        res_columns[i++]->insert(merge.elapsed);
        res_columns[i++]->insert(merge.progress);
        res_columns[i++]->insert(merge.num_parts);
        res_columns[i++]->insert(merge.source_part_names);
        res_columns[i++]->insert(merge.result_part_name);
        res_columns[i++]->insert(merge.total_size_bytes_compressed);
        res_columns[i++]->insert(merge.total_size_marks);
        res_columns[i++]->insert(merge.bytes_read_uncompressed);
        res_columns[i++]->insert(merge.rows_read);
        res_columns[i++]->insert(merge.bytes_written_uncompressed);
        res_columns[i++]->insert(merge.rows_written);
        res_columns[i++]->insert(merge.columns_written);
        res_columns[i++]->insert(merge.memory_usage);
        res_columns[i++]->insert(merge.thread_number);
    }

    return BlockInputStreams(1, std::make_shared<OneBlockInputStream>(getSampleBlock().cloneWithColumns(std::move(res_columns))));
}

}
