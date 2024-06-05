import json
# import websocket    # pip install websocket-client
from multiprocessing import Process
import datetime

LOCAL_URL = "http://127.0.0.1:8081"
PARALLEL_NUM = 16
ROUND_NUM = 5

def single_id_request(_id):
    import requests
    headers = {"Content-Type": "application/json", "cache-control": "no-cache"}
    data = {"request_id": "REQ"+str(_id)}
    res = requests.post("http://127.0.0.1:8081/get_from_buffer", data=json.dumps(data), headers=headers)
    buffer_res = res.json()
    print("late_fetching: ", buffer_res)
    return

def chat_request():
    import requests
    headers = {"Content-Type":"application/json", "cache-control":"no-cache"}
    data = {"query":"你可以做什么？", "history":[["你好", "你好，我是小PAI，是人工智能机器人。"]]}
    res = requests.post(LOCAL_URL+"/chat", data=json.dumps(data), headers=headers)
    print(res.json())

import requests
import random
def stream_post(inps):
    req_id, out_list, res_dict = inps[0], inps[1], inps[2]

    headers = {"Content-Type":"application/json", "cache-control":"no-cache"}
    data = {"query":"你可以做什么？", "history":[["你好", "你好，我是小PAI，是人工智能机器人。"]]} # "request_id":"REQ"+str(req_id)
    # data = {"query": "那你明年多大啊？", "history": [["你好，你多大了", f"我{req_id+1}岁啦！"]]}  # "request_id":"REQ"+str(req_id)
    # lora_name = "skip" if random.random() < 0.5 else "default"
    lora_name = "default"
    data.update({"gen_kwargs":{"max_length":256, "temperature":0.01, "adapter_name": lora_name}})
    print(f"submitted: {req_id}")
    stream_res = requests.post(LOCAL_URL+"/stream_chat_post", data=json.dumps(data), stream=True) # headers=headers,
    msg_ct = 0
    for res_raw in stream_res.iter_lines():
        # print(res_raw)
        # ============== json_str yield ===============
        if res_raw.startswith(b'data:'):
            res_ent = json.loads(res_raw[5:])
        else:
            res_ent = {}
        if req_id not in res_dict: res_dict[req_id] = []
        if len(res_ent) > 0 and "text" in res_ent:
            res_dict[req_id] = res_dict[req_id] + [res_ent["text"]]

        if "flag" in res_ent and res_ent["flag"] == "end":
            len_text = len(res_ent["text"])
            flat_text = res_ent["text"].replace("\n", "\\n")
            print(f"Reply Text(len={len_text}): {flat_text}")
            out_list.append(len_text)
        msg_ct += 1
        continue
    return 1

def normal_request():
    procs = []
    for _ in range(5):
        # print(name)
        proc = Process(target=chat_request, args=())
        procs.append(proc)
        proc.start()
    # complete the processes
    for proc in procs:
        proc.join()

def stream_post_request(round_num=1, parallel_num=1):
    procs = []
    from multiprocessing import Manager
    manager = Manager()
    overall_textlen = 0
    overall_timesum = 0.0
    while round_num > 0 or round_num < 0:
        out_list = manager.list([])
        stream_res_counter = manager.dict({})
        t0 = datetime.datetime.now()
        for _id in range(parallel_num):
            # print(name)
            proc = Process(target=stream_post, args=(tuple((_id, out_list, stream_res_counter)),))
            procs.append(proc)
            proc.start()

        # complete the processes
        for proc in procs:
            proc.join()

        textlens = out_list
        batch_textlen = sum(textlens)
        batch_time_sum = (datetime.datetime.now()-t0).total_seconds()
        overall_textlen += batch_textlen
        overall_timesum += batch_time_sum
        empty_res_num = 0
        fail_res_num = 0
        for _id in range(parallel_num):
            if _id in stream_res_counter:
                frame_num = len(stream_res_counter[_id])
                frame_textlens = [len(x) for x in stream_res_counter[_id]]
                # print("frames and their textlen:", _id, frame_num, frame_textlens)
                if (frame_num == 1 and frame_textlens[0] == 0):
                    empty_res_num += 1
                if (frame_num >= 1 and stream_res_counter[_id][-1].startswith("RUNTIME ERROR")):
                    fail_res_num +=1
            else:
                # print("frames:", _id, 0)
                empty_res_num += 1
            # if len(stream_res_counter[_id]) < 50:
            # if len(stream_res_counter[_id]) < 1:
            #     single_id_request(_id)
        round_num -= 1
        print(f"req_nums={parallel_num}, empty_result={empty_res_num}, failed_result={fail_res_num}")
        print(f"batch example num:{len(textlens)}, in {batch_time_sum} secs. lens={textlens}")
        print("char per sec: ", batch_textlen/batch_time_sum)

    print("=======final char per sec stat ========")
    print(overall_textlen/overall_timesum)

import time
def dummy_method(id):
    print(id)
    time.sleep(1.0)
    return 2

from concurrent.futures import ThreadPoolExecutor, as_completed, wait, ALL_COMPLETED
def stream_pool_request(round_num=1, parallel_num=1):

    total_reqs = round_num * parallel_num
    from multiprocessing import Manager
    manager = Manager()
    out_list = manager.list([])
    stream_res_counter = manager.dict({})

    overall_textlen = 0
    overall_timesum = 0.0

    # executor = ThreadPoolExecutor(max_workers=parallel_num)
    print(f"total_requests={total_reqs}")
    with ThreadPoolExecutor(max_workers=parallel_num) as executor:
        t0 = datetime.datetime.now()
        # tasks = [executor.submit(stream_post, (i, out_list, stream_res_counter)) for i in range(total_reqs)]
        tasks = [executor.submit(stream_post, tuple((i, out_list, stream_res_counter))) for i in range(total_reqs)]
        for future in as_completed(tasks):
            pass

        batch_time_sum = (datetime.datetime.now()-t0).total_seconds()
        textlens = out_list
        batch_textlen = sum(textlens)
        overall_textlen += batch_textlen
        overall_timesum += batch_time_sum
        empty_res_num = 0
        fail_res_num = 0
        for _id in range(parallel_num):
            if _id in stream_res_counter:
                frame_num = len(stream_res_counter[_id])
                frame_textlens = [len(x) for x in stream_res_counter[_id]]
                # print("frames and their textlen:", _id, frame_num, frame_textlens)
                if (frame_num == 1 and frame_textlens[0] == 0):
                    empty_res_num += 1
                if (frame_num >= 1 and stream_res_counter[_id][-1].startswith("RUNTIME ERROR")):
                    fail_res_num +=1
            else:
                # print("frames:", _id, 0)
                empty_res_num += 1
            # if len(stream_res_counter[_id]) < 50:
            # if len(stream_res_counter[_id]) < 1:
            #     single_id_request(_id)
        round_num -= 1
        print(f"req_nums={parallel_num}, empty_result={empty_res_num}, failed_result={fail_res_num}")
        print(f"batch example num:{len(textlens)}, in {batch_time_sum} secs. lens={textlens}")
        print("char per sec: ", batch_textlen/batch_time_sum)

        print("=======final char per sec stat ========")
        print(overall_textlen/overall_timesum)

if __name__ == "__main__":  # confirms that the code is under main function
    # stream_request()
    # normal_request()
    # stream_post_request(parallel_num=PARALLEL_NUM, round_num=ROUND_NUM)
    stream_pool_request(parallel_num=PARALLEL_NUM, round_num=ROUND_NUM)