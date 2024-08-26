//
// Created by 神圣•凯莎 on 24-7-30.
//

#pragma once

#include <kafka/KafkaConsumer.h>
#include <librdkafka/rdkafka.h>
#include <atomic>

using namespace drogon;
using namespace kafka;
using namespace kafka::clients::consumer;

class AsyncKafkaConsumerNew
{
public:
    explicit AsyncKafkaConsumerNew(const size_t numThreads = 4) : stop_(false)
    {
        try
        {
            // 启动多个后台线程来异步消费消息
            for (size_t i = 0; i < numThreads; ++i)
            {
                // 为每个线程创建独立的 Kafka 消费者实例
                KafkaConsumer *consumer = KafkaManagerNew::instance().createNewConsumer();
                consumers_.push_back(consumer);
                // 创建并启动 Kafka 消费线程
                consumerThreads_.emplace_back(&AsyncKafkaConsumerNew::consumeMessages, this, consumer);
            }
            LOG_INFO << "Kafka consumer started.";
        }
        catch (const std::exception &e)
        {
            LOG_ERROR << "Exception in AsyncKafkaConsumerNew constructor: " << e.what();
            // 可能需要进一步处理异常，例如重新尝试初始化
        }
    }

    ~AsyncKafkaConsumerNew()
    {
        stop_ = true;
        for (auto &thread : consumerThreads_)
        {
            if (thread.joinable())
            {
                thread.join(); // 等待线程完成
            }
        }
        for (const auto consumer : consumers_)
        {
            // rd_kafka_consumer_close(consumer);
            // rd_kafka_flush(consumer, 1000);
            // rd_kafka_destroy(consumer);
            consumer->close();
        }
        LOG_INFO << "Kafka consumer stopped.";
    }

private:
    void consumeMessages(KafkaConsumer *consumer_) const
    {
        const Topic topic = "my-first-topic"; // NOLINT

        // The consumer would read all messages from the topic and then quit.

        // Prepare the rebalance callbacks
        std::atomic<std::size_t> assignedPartitions{};
        auto rebalanceCb = [&assignedPartitions](kafka::clients::consumer::RebalanceEventType et, const kafka::TopicPartitions &tps)
        {
            if (et == kafka::clients::consumer::RebalanceEventType::PartitionsAssigned)
            {
                assignedPartitions += tps.size();
                std::cout << "Assigned partitions: " << kafka::toString(tps) << std::endl;
            }
            else
            {
                assignedPartitions -= tps.size();
                std::cout << "Revoked partitions: " << kafka::toString(tps) << std::endl;
            }
        };

        // Subscribe to topics with rebalance callback
        consumer_->subscribe({topic}, rebalanceCb);

        TopicPartitions finishedPartitions;
        while (finishedPartitions.size() != assignedPartitions.load())
        {
            // Poll messages from Kafka brokers
            auto records = consumer_->poll(std::chrono::milliseconds(100));

            for (const auto &record : records)
            {
                if (!record.error())
                {
                    std::cerr << record.toString() << std::endl;
                }
                else
                {
                    if (record.error().value() == RD_KAFKA_RESP_ERR__PARTITION_EOF)
                    {
                        // Record the partition which has been reached the end
                        finishedPartitions.emplace(record.topic(), record.partition());
                    }
                    else
                    {
                        std::cerr << record.toString() << std::endl;
                    }
                }
            }
        }

        // while (!stop_)
        // {
        //     if (rd_kafka_message_t *msg = rd_kafka_consumer_poll(consumer_, 1000)) // Poll every second
        //     {
        //         // 启动协程处理消息
        //         async_run([msg, consumer_, this]() -> Task<>
        //                   {
        //                       try
        //                       {
        //                           if (msg->err)
        //                           {
        //                               if (msg->err == RD_KAFKA_RESP_ERR__PARTITION_EOF)
        //                               {
        //                                   // 当前分区的消息已经消费完毕
        //                                   LOG_ERROR << "EReached end of partition.";
        //                               }
        //                               else
        //                               {
        //                                   LOG_ERROR << "Error consuming message: " << rd_kafka_err2str(msg->err);
        //                               }
        //                               rd_kafka_message_destroy(msg); // 释放消息资源
        //                               co_return;
        //                           }

        //                           const std::string message(static_cast<const char *>(msg->payload), msg->len);

        //                           // LOG_INFO << "收到消息 Received message: " << message;
        //                           co_await handleKafkaMessage(message);

        //                           // 处理完消息后手动提交偏移量
        //                           rd_kafka_commit_message(consumer_, msg, 0);
        //                       }
        //                       catch (const std::exception &ex)
        //                       {
        //                           LOG_ERROR << "Exception while processing message: " << ex.what();
        //                       }
        //                       rd_kafka_message_destroy(msg); // 释放消息资源
        //                   });
        //     }
        // }
    }

    // // 将处理 Kafka 消息的逻辑封装为协程
    // static Task<> handleKafkaMessage(const std::string &message)
    // {
    //     try
    //     {
    //         LOG_INFO << "Received message: " << message;
    //     }
    //     catch (const std::exception &ex)
    //     {
    //         throw std::invalid_argument(ex.what());
    //     }
    //     co_return;
    // }

    std::vector<std::thread> consumerThreads_; // Kafka 消费线程
    // std::vector<rd_kafka_t *> consumers_;      // Kafka 消费者实例
    std::vector<KafkaConsumer *> consumers_; // Kafka 消费者实例
    std::atomic<bool> stop_{false};          // 控制消费线程的停止
};